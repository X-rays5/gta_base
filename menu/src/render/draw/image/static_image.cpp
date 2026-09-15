//
// Created by X-ray on 13/09/2026.
//

#include "static_image.hpp"

namespace base::menu::render::draw {
  StaticImage::StaticImage(const std::uint32_t width, const std::uint32_t height, std::vector<std::uint8_t> pixels) :
    BaseImage(width, height, std::move(pixels)) {
  }
}
