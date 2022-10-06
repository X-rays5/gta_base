//
// Created by X-ray on 09/23/22.
//

#include "hotkey_manager.hpp"
#include <inireader/inireader.hpp>
#include <utility>
#include "common.hpp"
#include "../ui/manager.hpp"

namespace gta_base {
  namespace misc {
    HotkeyManager::HotkeyManager() {
      Load();
      kHOTKEY_MANAGER = this;
    }

    HotkeyManager::~HotkeyManager() {
      kHOTKEY_MANAGER = nullptr;
      Save();
      for (auto&& hotkey : hotkeys_) {
        RemoveHotkey(hotkey.first);
      }
    }

    void HotkeyManager::StartHotkeyAdd(const std::string& key_str) {
      if (adding_hotkey_expire_ <= common::GetEpoch()) {
        adding_hotkey_expire_ = common::GetEpoch() + 5000;
        adding_hotkey_name_ = key_str;
        LOG_DEBUG("Hotkey creation started for {}", key_str);
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Hotkey", "Press any key within 5 seconds to add a hotkey");
      }
    }

    bool HotkeyManager::AddKeyPressed(std::uint64_t key_id) {
      if (adding_hotkey_expire_ > common::GetEpoch() && key_id != VK_F1) {
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
      if (GetHotkeyKeyStr(hotkey_id).empty())
        return false;

      hotkeys_.erase(hotkeys_.find(hotkey_id));
      return true;
    }

    void HotkeyManager::KeyPressed(std::uint64_t key_id) {
      std::string key_str = GetHotkeyKeyStr(key_id);

      if (!key_str.empty()) {
        LOG_DEBUG("receiving hotkey");
        if (ui::kMANAGER->HotkeyPressed(key_str)) {
          ui::kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Hotkey", fmt::format("{} has been triggered", ui::kTRANSLATION_MANAGER->Get(key_str)));
        } else {
          ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Hotkey", fmt::format("failed to trigger {}", ui::kTRANSLATION_MANAGER->Get(key_str)));
          LOG_DEBUG("failed to trigger hotkey {}", key_str);
        }
      }
    }

    void HotkeyManager::Load() {
      auto path = common::GetSettingsDir() / "hotkeys.ini";
      if (!std::filesystem::is_regular_file(path))
        return;

      ini::Parser ini;
      ini.Parse(path);

      for (auto&& hotkey : ini.GetRootSection()) {
        if (!hotkey.second.is<std::int64_t>())
          continue;

        if (!AddHotkey(hotkey.first, hotkey.second.as<std::int64_t>()))
          LOG_WARN("Failed to load {} hotkey from hotkeys file", hotkey.first);
      }
    }

    void HotkeyManager::Save() {
      auto path = common::GetSettingsDir() / "hotkeys.ini";
      if (std::filesystem::is_regular_file(path))
        std::filesystem::remove(path);

      std::ofstream writer(path);

      if (writer.is_open()) {
        ini::Parser ini;
        for (auto&& hotkey : hotkeys_) {
          ini.GetRootSection().Add(hotkey.second, hotkey.first);
        }

        writer << ini.Stringify();
        writer.close();
      }
    }

    std::string HotkeyManager::GetHotkeyKeyStr(std::uint64_t hotkey_id) {
      auto entry = hotkeys_.find(hotkey_id);

      if (entry != hotkeys_.end()) {
        return entry->second;
      }

      return {};
    }
  }
}