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
namespace {
const std::filesystem::path selected_profiles_path = common::GetSettingsDir() / "selected_profiles.json";
std::recursive_mutex profile_mutex;

constexpr static const char *hotkey_profile_key = "hotkey_profile";
bool hotkey_profile_init = true;
std::string hotkey_profile_val;

constexpr static const char *option_state_profile_key = "option_state_profile";
bool option_state_profile_init = true;
std::string option_state_profile_val;

constexpr static const char *translation_key = "translation";
bool translation_init = true;
std::string translation_val;

constexpr static const char *theme_key = "theme";
bool theme_init = true;
std::string theme_val;
}

FORCE_INLINE void EnsureFile() {
  if (!std::filesystem::exists(selected_profiles_path)) {
    std::ofstream file(selected_profiles_path);
    file << "{}";
    file.close();
  }
}

FORCE_INLINE std::string GetSelectedProfileVal(const std::string &key) {
  std::unique_lock lock(profile_mutex);
  EnsureFile();
  auto doc = json::FromFile(selected_profiles_path);

  auto res = json::GetSafe<const char *>(doc.GetObj(), key);

  return res ? res : "";
}

FORCE_INLINE void SaveSelectedProfileVal(const std::string &key, const std::string &val) {
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

FORCE_INLINE std::string GetSelectedHotkeyProfile() {
  if (hotkey_profile_init) {
    hotkey_profile_val = GetSelectedProfileVal(hotkey_profile_key);
    hotkey_profile_init = false;
  }

  return hotkey_profile_val;
}

FORCE_INLINE void SetSelectedHotkeyProfile(const std::string &name) {
  hotkey_profile_val = name;
  SaveSelectedProfileVal(hotkey_profile_key, name);
}

FORCE_INLINE std::string GetSelectedOptionStateProfile() {
  if (option_state_profile_init) {
    option_state_profile_val = GetSelectedProfileVal(option_state_profile_key);
    option_state_profile_init = false;
  }

  return option_state_profile_val;
}
FORCE_INLINE void SetSelectedOptionProfile(const std::string &name) {
  option_state_profile_val = name;

  SaveSelectedProfileVal(option_state_profile_key, name);
}

FORCE_INLINE std::string GetSelectedTranslation() {
  if (translation_init) {
    translation_val = GetSelectedProfileVal(translation_key);
    translation_init = false;
  }

  return translation_val;
}
FORCE_INLINE void SetSelectedTranslation(const std::string &name) {
  translation_val = name;

  SaveSelectedProfileVal(translation_key, name);
}

FORCE_INLINE std::string GetSelectedTheme() {
  if (theme_init) {
    theme_val = GetSelectedProfileVal(theme_key);
    theme_init = false;
  }

  return theme_val;
}
FORCE_INLINE void SetSelectedTheme(const std::string &name) {
  theme_val = name;

  SaveSelectedProfileVal(theme_key, name);
}
}
#endif //GTA_BASE_PROFILE_HPP
