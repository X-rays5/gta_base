//
// Created by X-ray on 04/04/2025.
//

#include "settings.hpp"
#include <glaze/glaze.hpp>
#include <base-common/vfs.hpp>

namespace base::injector {
  StatusOr<Settings> LoadSettings() {
    std::string json_buff;
    Settings settings;

    const std::filesystem::path settings_file = common::vfs::GetSettingsDir() / "settings.json";
    if (!std::filesystem::is_regular_file(settings_file)) {
      LOG_INFO("No settings exist yet");
      return settings;
    }

    if (const auto ec = glz::read_file_json(settings, settings_file.string(), json_buff)) {
      return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buff));
    }

    return settings;
  }

  Status SaveSettings(Settings settings) {
    const std::filesystem::path settings_file = common::vfs::GetSettingsDir() / "settings.json";

    if (const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(settings, settings_file.string(), std::string{})) {
      return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, std::string{}));
    }

    return {};
  }
}
