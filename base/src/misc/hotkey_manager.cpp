//
// Created by X-ray on 09/23/22.
//

#include "hotkey_manager.hpp"
#include <utility>
#include "json.hpp"
#include "common.hpp"
#include "../ui/manager.hpp"
#include "../settings/profile.hpp"

namespace gta_base::misc {
  HotkeyManager::HotkeyManager() {
    kHOTKEY_MANAGER = this;
  }

  HotkeyManager::~HotkeyManager() {
    kHOTKEY_MANAGER = nullptr;
    if (!save_on_exit_)
      Save(settings::profile::GetSelectedHotkeyProfile());
  }

  void HotkeyManager::StartHotkeyAdd(const std::string& key_str) {
    if (adding_hotkey_expire_ <= common::GetEpoch()) {
      ui::kMANAGER->PushBlockInput();

      adding_hotkey_expire_ = common::GetEpoch() + 5000;
      adding_hotkey_name_ = key_str;
      LOG_DEBUG("Hotkey creation started for {}", key_str);
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Hotkey", "Press any key within 5 seconds to add a hotkey");
    }
  }

  bool HotkeyManager::AddKeyPressed(std::uint64_t key_id) {
    if (adding_hotkey_expire_ > common::GetEpoch() && key_id != VK_F1) {
      ui::kMANAGER->PopBlockInput();

      if (key_id == VK_ESCAPE) {
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("Canceled hotkey creation for {}", ui::kTRANSLATION_MANAGER->Get(adding_hotkey_name_)));
        adding_hotkey_expire_ = 0;
        return true;
      }

      if (AddHotkey(adding_hotkey_name_, key_id)) {
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", fmt::format("Hotkey for {} has been added", ui::kTRANSLATION_MANAGER->Get(adding_hotkey_name_)));
        adding_hotkey_expire_ = 0;
        return true;
      } else {
        adding_hotkey_expire_ = 0;
      }
    }

    return false;
  }

  bool HotkeyManager::AddHotkey(const std::string& key_str, std::uint64_t key_id, bool check_allowed) {
    if (hotkeys_reversed_.contains(key_str)) {
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("Hotkey for {} already exists", ui::kTRANSLATION_MANAGER->Get(adding_hotkey_name_)));
      return false;
    }

    if (hotkeys_.contains(key_id)) {
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("A hotkey with the key {} already exists", common::VkToStr(key_id)));
      return false;
    }

    if (check_allowed && !ui::kMANAGER->IsOptionHotkeyAble(key_str)) {
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("{} doesn't support hotkeys", ui::kTRANSLATION_MANAGER->Get(adding_hotkey_name_)));
      return false;
    }

    hotkeys_[key_id] = key_str;
    hotkeys_reversed_[key_str] = key_id;

    LOG_INFO("Created hotkey for {} with id {}", ui::kTRANSLATION_MANAGER->Get(key_str), common::VkToStr(key_id));

    // check if running
    ui::kMANAGER->UpdateCurOptHintTxt();

    if (save_on_change_)
      Save(settings::profile::GetSelectedHotkeyProfile());

    return true;
  }

  bool HotkeyManager::RemoveHotkey(std::uint64_t hotkey_id) {
    auto hotkey_name = GetHotkeyKeyStr(hotkey_id);
    if (hotkey_name.empty())
      return false;

    hotkeys_.erase(hotkey_id);
    hotkeys_reversed_.erase(hotkey_name);
    ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", fmt::format("{} has been deleted", ui::kTRANSLATION_MANAGER->Get(hotkey_name)));
    LOG_INFO("Deleted hotkey: {}", hotkey_name);

    if (save_on_change_)
      Save(settings::profile::GetSelectedHotkeyProfile());

    return true;
  }

  void HotkeyManager::KeyPressed(std::uint64_t key_id) {
    std::string key_str = GetHotkeyKeyStr(key_id);

    if (!key_str.empty()) {
      if (auto press = ui::kMANAGER->HotkeyPressed(key_str); press.has_value()) {
        std::string msg;
        if (press->is_toggle)
          msg = fmt::format("{} is now {}", ui::kTRANSLATION_MANAGER->Get(key_str), press->toggle_state);
        else
          msg = fmt::format("{} has been triggered.", ui::kTRANSLATION_MANAGER->Get(key_str));

        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", msg);
      } else {
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("failed to trigger {}", ui::kTRANSLATION_MANAGER->Get(key_str)));
        LOG_DEBUG("failed to trigger hotkey {}", key_str);
      }
    }
  }

  void HotkeyManager::Load(const std::string& name) {
    if (name.empty())
      return;

    auto path = common::GetHotkeysDir() / (name + ".json");
    if (!std::filesystem::is_regular_file(path)) {
      LOG_ERROR("Tried to load non-existent hotkey profile {}", path.string());
      return;
    }

    auto json = json::FromFile(path);
    if (!json.IsObject() || !json.HasMember("save_on_change") || !json.HasMember("save_on_exit") || !json.HasMember("hotkeys")) {
      LOG_ERROR("Failed to load hotkey malformed profile {}", path.string());
      return;
    }

    hotkeys_.clear();
    hotkeys_reversed_.clear();

    for (auto&& hotkey : json["hotkeys"].GetObject()) {
      if (!hotkey.value.Is<std::int64_t>())
        continue;

      if (!AddHotkey(hotkey.name.GetString(), hotkey.value.Get<std::int64_t>(), false))
        LOG_WARN("Failed to load {} hotkey from hotkeys file", hotkey.name.GetString());
    }

    // only set it here to avoid issues with saving while loading
    save_on_change_ = json::GetSafe<bool>(json.GetObj(), "save_on_change");
    save_on_exit_ = json::GetSafe<bool>(json.GetObj(), "save_on_exit");

    settings::profile::SetSelectedHotkeyProfile(name);
  }

  void HotkeyManager::Save(const std::string& name) {
    rapidjson::Document json;
    json.SetObject();

    json.AddMember("save_on_change", save_on_change_, json.GetAllocator());
    json.AddMember("save_on_exit", save_on_exit_, json.GetAllocator());

    rapidjson::Value hotkeys(rapidjson::kObjectType);
    for (auto&& hotkey : hotkeys_) {
      auto key = json::StringToJsonVal(hotkey.second, json.GetAllocator());
      hotkeys.AddMember(key, hotkey.first, json.GetAllocator());
    }
    json.AddMember("hotkeys", hotkeys, json.GetAllocator());

    json::ToFile(json, common::GetHotkeysDir() / (name + ".json"));
  }

  std::string HotkeyManager::GetHotkeyKeyStr(std::uint64_t hotkey_id) {
    auto entry = hotkeys_.find(hotkey_id);

    if (entry != hotkeys_.end()) {
      return entry->second;
    }

    return {};
  }

  robin_hood::unordered_map<std::uint64_t, std::string> HotkeyManager::GetAllHotkeys() const {
    return hotkeys_;
  }

  std::vector<std::filesystem::path> HotkeyManager::GetHotkeyProfileList() {
    std::vector<std::filesystem::path> profiles;

    for (auto&& file : std::filesystem::directory_iterator(common::GetHotkeysDir())) {
      if (file.is_regular_file() && file.path().extension() == ".json")
        profiles.push_back(file.path());
    }

    return profiles;
  }
}