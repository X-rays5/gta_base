//
// Created by X-ray on 28/03/2026.
//

#include "hotkey_manager.hpp"
#include "../options/base_option.hpp"
#include <base-common/fs/vfs.hpp>
#include <enchantum/enchantum.hpp>

#include "../options/option_registry.hpp"

namespace base::menu::hotkey {
  namespace {
    std::filesystem::path GetHotkeySavePath() {
      return common::fs::vfs::GetSettingsDir() / "hotkeys.json";
    }

    bool ShouldIgnoreKey(const std::uint32_t vk_key) {
      switch (vk_key) {
        case VK_LWIN:
        case VK_RWIN:
        case VK_APPS:
        case VK_LSHIFT:
        case VK_RSHIFT:
        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_LMENU:
        case VK_RMENU:
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        case VK_F11:
        case VK_F12:
          return true;
        default:
          return false;
      }
    }
  }

  HotkeyManager::HotkeyManager() : key_event_listener_id_(util::kKEY_EVENT_WATCHER->AddKeyEventListener(this)) {
    Load();

    kHOTKEY_MANAGER = this;
  }

  HotkeyManager::~HotkeyManager() {
    if (util::kKEY_EVENT_WATCHER) {
      util::kKEY_EVENT_WATCHER->RemoveKeyEventListener(key_event_listener_id_);
    }

    kHOTKEY_MANAGER = nullptr;
  }

  void HotkeyManager::AddNewHotkey(std::shared_ptr<options::BaseOption> option) {
    // Nothing that guards the map may be held across the wait below: it lasts as long as the player
    // takes to press a key, and the render thread asks this manager for the key bound to the selected
    // option on every frame it draws. Holding the lock for the duration stalls the render thread for
    // the same length of time, which is long enough that the "press a key" notification raised here is
    // only drawn once the key has already been pressed and the hotkey made.
    is_adding_hotkey_ = true;
    NOTIFY_INFO("ui/hotkey", "ui/hotkey/press_key");
    const auto res = new_hotkey_signal_.Wait(5000);
    if (res == WAIT_TIMEOUT) {
      is_adding_hotkey_ = false;
      NOTIFY_WARN("ui/hotkey", "ui/hotkey/press_key_timeout");
      return;
    }

    if (res != WAIT_OBJECT_0) {
      // Cleared here as well, or every key pressed afterwards would be taken for an attempt to add one.
      is_adding_hotkey_ = false;
      NOTIFY_INFO("ui/hotkey", "ui/hotkey/add_failed");
      LOG_ERROR("Failed to add hotkey, wait failed with code {}", win32::GetLastErrorStr());
      return;
    }

    NOTIFY_INFO("ui/hotkey", "ui/hotkey/add_success", option->GetName(), new_hotkey_.AsString());
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);
    key_opt_map_.emplace(new_hotkey_, option);
    lock.Unlock();

    Save();
  }

  void HotkeyManager::RemoveHotkey(const Hotkey& hotkey) {
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);

    const auto it = key_opt_map_.find(hotkey);
    if (it != key_opt_map_.end()) {
      NOTIFY_INFO("ui/hotkey", "ui/hotkey/remove_success", it->second->GetName(), hotkey.AsString());
      key_opt_map_.erase(it);
      Save();
    } else {
      NOTIFY_WARN("ui/hotkey", "ui/hotkey/remove_not_found", hotkey.AsString());
    }
  }

  std::vector<std::pair<Hotkey, std::shared_ptr<options::BaseOption>>> HotkeyManager::GetAllHotkeys() const {
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);

    std::vector<std::pair<Hotkey, std::shared_ptr<options::BaseOption>>> hotkeys;
    hotkeys.reserve(key_opt_map_.size());
    for (const auto& [hotkey, option] : key_opt_map_) {
      hotkeys.emplace_back(hotkey, option);
    }
    return hotkeys;
  }

  std::optional<Hotkey> HotkeyManager::GetHotkeyForOption(const std::string& option_name) const {
    // The same lock the rest of the map is read and written under, since this is called from the render
    // thread while a key can be bound on whichever thread the player pressed F12 on.
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);

    for (const auto& [hotkey, option] : key_opt_map_) {
      // By name, which is what an option is identified by everywhere else - the registry, the save file,
      // and this map when the key was added. A null entry is not expected, but skipping one costs
      // nothing next to dereferencing it.
      if (option && option->GetName() == option_name) {
        return hotkey;
      }
    }

    return std::nullopt;
  }

  void HotkeyManager::KeyDown(const std::uint32_t vk_key, const ModifierKey modifier) {
    if (ShouldIgnoreKey(vk_key)) {
      return;
    }

    if (is_adding_hotkey_) {
      KeyForRegistration(vk_key, modifier);
      return;
    }

    const Hotkey hotkey(vk_key, modifier);
    if (const auto it = key_opt_map_.find(hotkey); it != key_opt_map_.end() && it->second) {
      NOTIFY_INFO("ui/hotkey", "ui/hotkey/activated", it->second->GetName());
      it->second->HandleHotkey();
    }
  }

  void HotkeyManager::KeyUp(std::uint32_t, ModifierKey) {
   // don't care
  }

  void HotkeyManager::KeyForRegistration(const std::uint32_t vk_key, const ModifierKey modifier) {
    new_hotkey_ = Hotkey(vk_key, modifier);
    new_hotkey_signal_.Notify();
    is_adding_hotkey_ = false;
  }

  void HotkeyManager::Save() {
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);

    glz::generic data;
    for (auto&& it : key_opt_map_) {
      glz::generic hotkey_data;
      hotkey_data["vk_key"] = it.first.vk_key;
      hotkey_data["modifier"] = static_cast<std::uint32_t>(it.first.modifier);
      data[it.second->GetName()] = hotkey_data;
    }

    auto ec = glz::write_file_json<glz::opts{.prettify = true}>(data, GetHotkeySavePath().string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to save hotkey data: {}", ec);
    }
  }

  void HotkeyManager::Load() {
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);

    const auto hotkey_path = GetHotkeySavePath();
    if (!std::filesystem::exists(hotkey_path)) {
      LOG_INFO("No hotkey save file found at '{}', skipping load", GetHotkeySavePath().string());
      return;
    }

    glz::generic data;
    const auto ec = glz::read_file_json(data, hotkey_path.string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to load hotkey data: {}", ec);
      return;
    }

    if (!data.is_object()) {
      LOG_ERROR("Invalid hotkey data format, expected JSON object at root");
      return;
    }

    for (auto&& it : data.get_object()) {
      const std::string name = it.first;
      if (!it.second.contains("vk_key") || !it.second.contains("modifier")) {
        LOG_ERROR("Invalid hotkey entry for option '{}', missing 'vk_key' or 'modifier'", name);
        continue;
      }

      if (!it.second["vk_key"].is_number() || !it.second["modifier"].is_number()) {
        LOG_ERROR("Invalid hotkey entry for option '{}', 'vk_key' and 'modifier' must be unsigned integers", name);
        continue;
      }

      const std::uint32_t vk_key = it.second["vk_key"].as<std::uint32_t>();
      const auto modifier = enchantum::cast<ModifierKey>(it.second["modifier"].as<std::uint32_t>());
      if (!modifier.has_value()) {
        LOG_ERROR("Invalid modifier value for hotkey entry of option '{}'", name);
        continue;
      }

      const Hotkey hotkey(vk_key, modifier.value());
      const auto opt = options::kOPTION_REGISTRY->GetOption(name);
      if (!opt) {
        LOG_ERROR("Unknown option '{}'", name);
      } else {
        key_opt_map_.emplace(hotkey, opt);
      }
    }

    LOG_INFO("Loaded {} hotkeys", key_opt_map_.size());
  }
}
