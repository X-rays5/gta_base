//
// Created by X-ray on 28/03/2026.
//

#include "hotkey_manager.hpp"
#include "../options/base_option.hpp"
#include <base-common/fs/vfs.hpp>

#include "../options/option_registry.hpp"

namespace base::menu::hotkey {
  namespace {
    std::filesystem::path GetHotkeySavePath() {
      return common::fs::vfs::GetSettingsDir() / "hotkeys.json";
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
    common::concurrency::ScopedSpinlock lock(add_hotkey_lock_);
    is_adding_hotkey_ = true;

    NOTIFY_INFO("ui/hotkey", "ui/hotkey/press_key");
    const auto res = new_hotkey_signal_.Wait(5000);
    if (res == WAIT_TIMEOUT) {
      NOTIFY_WARN("ui/hotkey", "ui/hotkey/press_key_timeout");
      is_adding_hotkey_ = false;
      return;
    }

    if (res != WAIT_OBJECT_0) {
      NOTIFY_INFO("ui/hotkey", "ui/hotkey/add_failed");
      LOG_ERROR("Failed to add hotkey, wait failed with code {}", win32::GetLastErrorStr());
      return;
    }

    NOTIFY_INFO("ui/hotkey", "ui/hotkey/add_success", option->GetName(), static_cast<std::uint32_t>(new_hotkey_.modifier), new_hotkey_.vk_key);
    key_opt_map_.emplace(new_hotkey_, option);

    Save();
  }

  void HotkeyManager::KeyDown(const std::uint32_t vk_key, const ModifierKey modifier) {
    if (is_adding_hotkey_) {
      KeyForRegistration(vk_key, modifier);
      return;
    }

    const Hotkey hotkey(vk_key, modifier);
    const auto it = key_opt_map_.find(hotkey);
    if (it != key_opt_map_.end() && it->second) {
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
      const auto modifier = magic_enum::enum_cast<ModifierKey>(it.second["modifier"].as<std::uint32_t>());
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
