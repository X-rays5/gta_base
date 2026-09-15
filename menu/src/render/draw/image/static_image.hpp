//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include "base_image.hpp"

namespace base::menu::render::draw {
  /// A single-frame image.
  class StaticImage final : public BaseImage {
  public:
    StaticImage(std::uint32_t width, std::uint32_t height, std::vector<std::uint8_t> pixels);

    [[nodiscard]] ImVec2 GetUvMin() const override {
      return {0.F, 0.F};
    }

    [[nodiscard]] ImVec2 GetUvMax() const override {
      return {1.F, 1.F};
    }

    [[nodiscard]] ImVec2 GetFrameSize() const override {
      return {static_cast<float>(GetWidth()), static_cast<float>(GetHeight())};
    }
  };
}
