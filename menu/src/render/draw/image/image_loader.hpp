//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include <filesystem>
#include <span>
#include <base-common/util/result.hpp>
#include "base_image.hpp"

#undef LoadImage

namespace base::menu::render::draw::image_loader {
  /**
   * Decodes an image into something the draw queue can render.
   *
   * Every format stb_image reads is accepted - PNG, JPG, BMP, TGA, PSD, PNM and GIF - and a GIF
   * with more than one frame comes back as an AnimatedImage.
   */
  StatusOr<std::shared_ptr<BaseImage>> LoadImage(const std::filesystem::path& path);

  /// The same, for something already in memory.
  StatusOr<std::shared_ptr<BaseImage>> LoadImageFromMemory(std::span<const std::uint8_t> data);
}
