//
// Created by X-ray on 31/05/2025.
//

#include "theme.hpp"
#include <base-common/fs/vfs.hpp>
#include <glaze/glaze.hpp>

namespace base::menu::ui {
  Status Theme::Save(const std::string& theme_name) {
    if (const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(*this, (common::fs::vfs::GetUIThemeDir() / fmt::format("{}.json", theme_name)).string(), std::string{})) {
      return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, std::string{}));
    }

    return {};
  }

  Status Theme::Load(const std::string& theme_name) {
    std::string json_buffer;
    if (const auto ec = glz::read_file_json(*this, (common::fs::vfs::GetUIThemeDir() / fmt::format("{}.json", theme_name)).string(), json_buffer)) {
      return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buffer));
    }

    return {};
  }
}
