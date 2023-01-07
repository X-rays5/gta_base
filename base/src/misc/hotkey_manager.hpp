//
// Created by X-ray on 09/23/22.
//

#pragma once
#ifndef GTA_BASE_HOTKEY_MANAGER_HPP
#define GTA_BASE_HOTKEY_MANAGER_HPP
#include <string>
#include <optional>
#include <robin_hood.h>

namespace gta_base::misc {
class HotkeyManager {
 public:
  struct HotkeyPressState {
    bool is_toggle = false;
    bool toggle_state = false;
  };
  using HotkeyPressResult = std::optional<HotkeyPressState>;

 public:
  HotkeyManager();
  ~HotkeyManager();

  void StartHotkeyAdd(const std::string &key_str);
  bool AddKeyPressed(std::uint64_t key_id);

  bool IsHotkey(const std::string &key_str) {
    return hotkey_list_.contains(key_str);
  }
  bool AddHotkey(const std::string &key_str, std::uint64_t key_id);
  bool RemoveHotkey(std::uint64_t hotkey_id);

  void KeyPressed(std::uint64_t key_id);

  void Load(const std::string &name);
  void Save(const std::string &name);

  inline void SetSaveOnExit(bool save_on_exit) {
    save_on_exit_ = save_on_exit;
    save_on_change_ = !save_on_exit;
  }

  [[nodiscard]] inline bool GetSaveOnExit() const {
    return save_on_exit_;
  }

  inline void SetSaveOnChange(bool save_on_add) {
    save_on_change_ = save_on_add;
    save_on_exit_ = !save_on_add;
  }

  [[nodiscard]] inline bool GetSaveOnChange() const {
    return save_on_change_;
  }

  [[nodiscard]] robin_hood::unordered_map<std::uint64_t, std::string> GetAllHotkeys() const;

  static std::vector<std::filesystem::path> GetHotkeyProfileList();
 private:
  bool save_on_exit_ = false;
  bool save_on_change_ = true;
  uint64_t adding_hotkey_expire_ = 0;
  std::string adding_hotkey_name_;
  robin_hood::unordered_set<std::string> hotkey_list_;

  robin_hood::unordered_map<std::uint64_t, std::string> hotkeys_;
 private:

  std::string GetHotkeyKeyStr(std::uint64_t hotkey_id);
};
inline HotkeyManager *kHOTKEY_MANAGER{};
}
#endif //GTA_BASE_HOTKEY_MANAGER_HPP
