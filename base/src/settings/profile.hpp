//
// Created by X-ray on 12/16/22.
//

#pragma once
#ifndef GTA_BASE_PROFILE_HPP
#define GTA_BASE_PROFILE_HPP
#include <filesystem>
#include <string>
#include "../misc/json.hpp"

namespace gta_base::settings::profile {
  static const std::filesystem::path selected_profiles_path = common::GetSettingsDir() / "selected_profiles.json";
  static std::recursive_mutex profile_mutex;

  inline void EnsureFile() {
    if (!std::filesystem::exists(selected_profiles_path)) {
      std::ofstream file(selected_profiles_path);
      file << "{}";
      file.close();
    }
  }

  inline std::string GetSelectedProfileVal(const std::string& key) {
    std::unique_lock lock(profile_mutex);
    EnsureFile();
    auto doc = json::FromFile(selected_profiles_path);

    auto res = json::GetSafe<const char*>(doc.GetObj(), key);

    return res ? res : "";
  }

  inline void SaveSelectedProfileVal(const std::string& key, const std::string& val) {
    std::unique_lock lock(profile_mutex);
    EnsureFile();
    auto doc = json::FromFile(selected_profiles_path);

    if (doc.HasMember(key.c_str())) {
      doc.RemoveMember(key.c_str());
    }

    auto key_tmp = json::StringToJsonVal(key, doc.GetAllocator());
    auto val_tmp = json::StringToJsonVal(val, doc.GetAllocator());
    doc.AddMember(key_tmp, val_tmp, doc.GetAllocator());

    json::ToFile(doc, selected_profiles_path);
  }

  constexpr static const char* hotkey_profile_key = "hotkey_profile";
  inline std::string GetSelectedHotkeyProfile() {
    return GetSelectedProfileVal(hotkey_profile_key);
  }
  inline void SetSelectedHotkeyProfile(const std::string& path) {
    SaveSelectedProfileVal(hotkey_profile_key, path);
  }

  constexpr static const char* option_state_profile_key = "option_state_profile";
  inline std::string GetSelectedOptionStateProfile() {
    return GetSelectedProfileVal(option_state_profile_key);
  }
  inline void SetSelectedOptionProfile(const std::string& name) {
    SaveSelectedProfileVal(option_state_profile_key, name);
  }

  constexpr static const char* translation_key = "translation";
  inline std::string GetSelectedTranslation() {
    return GetSelectedProfileVal(translation_key);
  }
  inline void SetSelectedTranslation(const std::string& name) {
    SaveSelectedProfileVal(translation_key, name);
  }

  constexpr static const char* theme_key = "theme";
  inline std::string GetSelectedTheme() {
    return GetSelectedProfileVal(theme_key);
  }
  inline void SetSelectedTheme(const std::string& name) {
    SaveSelectedProfileVal(theme_key, name);
  }
}
#endif //GTA_BASE_PROFILE_HPP
