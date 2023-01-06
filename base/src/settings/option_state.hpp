//
// Created by X-ray on 01/06/23.
//

#pragma once
#ifndef GTA_BASE_OPTION_STATE_HPP
#define GTA_BASE_OPTION_STATE_HPP
#include <string>
#include <robin_hood.h>
#include "profile.hpp"

namespace gta_base::settings::option_state {
  FORCE_INLINE std::filesystem::path GetSavePath() {
    auto profile = profile::GetSelectedOptionStateProfile();
    if (profile.empty())
      profile = "default";
    profile.append(".json");

    return common::GetOptionsStateDir() / profile;
  }

  void Save(const std::filesystem::path& file);
  void SaveSingle(const std::filesystem::path& file, const std::string& key, const std::string& val);
  void Load(const std::filesystem::path& file);
}
#endif //GTA_BASE_OPTION_STATE_HPP
