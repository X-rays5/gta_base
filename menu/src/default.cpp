//
// Created by X-ray on 27/03/2026.
//

#include "default.hpp"
#include <base-common/fs/vfs.hpp>

namespace base::menu {
  namespace {
    std::filesystem::path GetDefaultProfilesFilePath() {
      return common::fs::vfs::GetSettingsDir() / "defaults.json";
    }
  }

  Default::Default() {
    if (!std::filesystem::exists(GetDefaultProfilesFilePath())) {
      Save();
    } else {
      Load();
    }

    kDEFAULT_PROFILES = this;
  }

  Default::~Default() {
    kDEFAULT_PROFILES = nullptr;
  }

  void Default::Save() {
    const auto file = GetDefaultProfilesFilePath();
    auto ec = glz::write_file_json(default_profiles_, file.string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to save default profiles to file: {}", ec);
    }
  }

  void Default::Load() {
    const auto file = GetDefaultProfilesFilePath();
    auto ec = glz::read_file_json(default_profiles_, file.string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to load default profiles from file: {}", ec);
    }
  }
}
