//
// Created by X-ray on 09/23/22.
//

#include "hotkey_manager.hpp"
#include <utility>
#include "json.hpp"
#include "common.hpp"
#include "../ui/manager.hpp"

namespace gta_base::misc {
    HotkeyManager::HotkeyManager() {
      Load();
      kHOTKEY_MANAGER = this;
    }

    HotkeyManager::~HotkeyManager() {
      kHOTKEY_MANAGER = nullptr;
      Save();
    }

    void HotkeyManager::StartHotkeyAdd(const std::string& key_str) {
      if (adding_hotkey_expire_ <= common::GetEpoch()) {
        globals::block_input = true;

        adding_hotkey_expire_ = common::GetEpoch() + 5000;
        adding_hotkey_name_ = key_str;
        LOG_DEBUG("Hotkey creation started for {}", key_str);
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Hotkey", "Press any key within 5 seconds to add a hotkey");
      }
    }

    bool HotkeyManager::AddKeyPressed(std::uint64_t key_id) {
      if (adding_hotkey_expire_ > common::GetEpoch() && key_id != VK_F1) {
        globals::block_input = false;

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
          ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("Hotkey for {} already exists", ui::kTRANSLATION_MANAGER->Get(adding_hotkey_name_)));
          adding_hotkey_expire_ = 0;
        }
      }

      return false;
    }

    bool HotkeyManager::AddHotkey(std::string key_str, std::uint64_t key_id) {
      auto entry = hotkeys_.find(key_id);
      if (entry == hotkeys_.end()) {
        hotkeys_[key_id] = std::move(key_str);
        return true;
      }

      return false;
    }

    bool HotkeyManager::RemoveHotkey(std::uint64_t hotkey_id) {
      auto hotkey_name = GetHotkeyKeyStr(hotkey_id);
      if (hotkey_name.empty())
        return false;

      hotkeys_.erase(hotkeys_.find(hotkey_id));
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", fmt::format("{} has been deleted", ui::kTRANSLATION_MANAGER->Get(hotkey_name)));
      LOG_INFO("Deleted hotkey: {}", hotkey_name);

      return true;
    }

    void HotkeyManager::KeyPressed(std::uint64_t key_id) {
      std::string key_str = GetHotkeyKeyStr(key_id);

      if (!key_str.empty()) {
        if (ui::kMANAGER->HotkeyPressed(key_str)) {
          ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", fmt::format("{} has been triggered", ui::kTRANSLATION_MANAGER->Get(key_str)));
        } else {
          ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("failed to trigger {}", ui::kTRANSLATION_MANAGER->Get(key_str)));
          LOG_DEBUG("failed to trigger hotkey {}", key_str);
        }
      }
    }

    void HotkeyManager::Load() {
      auto path = common::GetSettingsDir() / "hotkeys.json";
      if (!std::filesystem::is_regular_file(path))
        return;

      auto json = json::FromFile(path);
      if (!json.IsObject())
        return;

      for (auto&& hotkey : json.GetObject()) {
        if (!hotkey.value.Is<std::int64_t>())
          continue;

        if (!AddHotkey(hotkey.name.GetString(), hotkey.value.Get<std::int64_t>()))
          LOG_WARN("Failed to load {} hotkey from hotkeys file", hotkey.name.GetString());
      }
    }

    void HotkeyManager::Save() {
      auto path = common::GetSettingsDir() / "hotkeys.json";

      std::ofstream writer(path);

      rapidjson::Document json;
      json.SetObject();

      for (auto&& hotkey : hotkeys_) {
        auto key = json::StringToJsonVal(hotkey.second, json.GetAllocator());
        json.AddMember(key, hotkey.first, json.GetAllocator());
      }

      json::ToFile(json, path);
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
}