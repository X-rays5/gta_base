//
// Created by X-ray on 09/23/22.
//

#pragma once
#ifndef GTA_BASE_HOTKEY_MANAGER_HPP
#define GTA_BASE_HOTKEY_MANAGER_HPP
#include <string>
#include <robin_hood.h>

namespace gta_base {
  namespace misc {
    class HotkeyManager {
    public:
      HotkeyManager();
      ~HotkeyManager();


      void StartHotkeyAdd(const std::string& key_str);
      bool AddKeyPressed(std::uint64_t key_id);

      bool AddHotkey(std::string key_str, std::uint64_t key_id);
      bool RemoveHotkey(std::uint64_t hotkey_id);

      void KeyPressed(std::uint64_t key_id);

    private:
      uint64_t adding_hotkey_expire_ = 0;
      std::string adding_hotkey_name_;
      robin_hood::unordered_map<std::uint64_t , std::string> hotkeys_;

    private:
      void Load();
      void Save();

      std::string GetHotkeyKeyStr(std::uint64_t hotkey_id);
    };
    inline HotkeyManager* kHOTKEY_MANAGER{};
  }
}
#endif //GTA_BASE_HOTKEY_MANAGER_HPP