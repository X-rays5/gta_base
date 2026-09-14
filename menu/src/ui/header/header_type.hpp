//
// Created by X-ray on 14/09/2026.
//

#pragma once
#include <cstdint>
#include <memory>
#include <string_view>

namespace base::menu::ui {
  class BaseHeader;

  /**
   * Which of the headers the menu draws, and what a theme stores to ask for one. The type is the whole
   * choice: a header of that type is drawn however little it ends up drawing, rather than another type
   * being put in its place.
   */
  enum class HeaderType : std::uint8_t {
    kText,
    kImage,
  };

  /**
   * Builds the header the theme asks for.
   *
   * @param type The header to build. Anything that is not kImage is the text header, which is what a
   *             theme gets whatever it stored.
   * @param image_path The image for kImage, resolved as header_image::ResolvePath does. Empty, or a
   *                   path that does not load, leaves that header with nothing to draw.
   */
  std::unique_ptr<BaseHeader> MakeHeader(HeaderType type, std::string_view image_path);
}
