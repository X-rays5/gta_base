//
// Created by X-ray on 13/09/2026.
//

#include "base_image.hpp"

namespace base::menu::render::draw {
  BaseImage::BaseImage(const std::uint32_t width, const std::uint32_t height, std::vector<std::uint8_t> pixels) :
    width_(width),
    height_(height),
    pixels_(std::move(pixels)) {
  }

  ImTextureID BaseImage::GetTextureId() {
    if (!texture_.IsValid() && !pixels_.empty()) {
      if (texture_.Create(pixels_, width_, height_)) {
        // The GPU owns a copy now, and the pixels are the largest thing this object holds.
        pixels_.clear();
        pixels_.shrink_to_fit();
      }
    }

    return texture_.IsValid() ? static_cast<ImTextureID>(texture_.GetGpuHandle().ptr) : ImTextureID_Invalid;
  }
}
