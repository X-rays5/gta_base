//
// Created by X-ray on 14/09/2026.
//

#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace base::menu::render::draw {
  class BaseImage;
}

namespace base::menu::ui::header_image {
  /**
   * Where a configured header image is read from: the images directory, unless the path is already
   * absolute. Empty for no image.
   */
  std::filesystem::path ResolvePath(std::string_view configured);

  /// The image file names in dir that the loader can read, sorted. Missing dir gives none.
  std::vector<std::string> ListAvailable(const std::filesystem::path& dir);

  /**
   * The image a theme names.
   *
   * @returns Null when the setting is empty or the file does not load. Null draws nothing, which is
   *          the whole fallback: a header image that is not there leaves the header blank rather
   *          than turning it back into the text header.
   */
  std::shared_ptr<render::draw::BaseImage> Load(std::string_view configured);
}
