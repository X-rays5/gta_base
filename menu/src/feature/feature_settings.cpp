//
// Created by X-ray on 27/03/2026.
//

#include "feature_settings.hpp"
#include <base-common/fs/vfs.hpp>

namespace base::menu::feature {
  namespace {
    std::filesystem::path GetSettingsFilename(const std::string& profileName) {
      return common::fs::vfs::GetSettingsDir() / fmt::format("feature.{}.json", profileName);
    }
  }

  Settings::Settings() : settings_data_(std::make_shared<SettingsData>()) {
    auto status = Load("default");
    kSETTINGS = this;
  }

  Settings::~Settings() {
    kSETTINGS = nullptr;
  }

  Status Settings::Load(const std::string& profileName) {
    LOG_INFO("Loading settings profile '{}'", profileName);

    const auto path = GetSettingsFilename(profileName);
    if (!std::filesystem::exists(path)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Settings profile '{}' not found at path: {}", profileName, path.string());
    }

    const auto newSettings = std::make_shared<SettingsData>();
    const auto ec = glz::read_file_json(*newSettings, path.string(), std::string{});
    if (ec) {
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to read settings profile '{}': {}", profileName, ec);
    }

    settings_data_ = newSettings;
    return {};
  }

  Status Settings::Save(const std::string& profileName) const {
    LOG_INFO("Saving settings profile '{}'", profileName);

    const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(settings_data_, GetSettingsFilename(profileName).string(), std::string{});
    if (ec) {
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to write settings profile '{}': {}", profileName, ec);
    }

    return {};
  }

  std::weak_ptr<SettingsData> Settings::getSettings() {
    return settings_data_;
  }
}