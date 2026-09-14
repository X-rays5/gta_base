//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include <cstdint>
#include <imgui.h>
#include <vector>
#include "../../d3d12/texture.hpp"

namespace base::menu::render::draw {
  /**
   * An image and the GPU texture behind it.
   *
   * Decoding happens up front but the upload on first use, because the D3D12 device only exists
   * once the renderer is up and images can be built before that.
   */
  class BaseImage {
  public:
    virtual ~BaseImage() = default;

    BaseImage(const BaseImage&) = delete;
    BaseImage(BaseImage&&) = delete;
    BaseImage& operator=(const BaseImage&) = delete;
    BaseImage& operator=(BaseImage&&) = delete;

    /// For ImGui's draw commands. Uploads on first call, so render thread only.
    /// @returns ImTextureID_Invalid if there is nothing to draw with.
    ImTextureID GetTextureId();

    /// UV rect of the frame to draw, and its size in pixels.
    [[nodiscard]] virtual ImVec2 GetUvMin() const = 0;
    [[nodiscard]] virtual ImVec2 GetUvMax() const = 0;
    [[nodiscard]] virtual ImVec2 GetFrameSize() const = 0;

    [[nodiscard]] std::uint32_t GetWidth() const {
      return width_;
    }

    [[nodiscard]] std::uint32_t GetHeight() const {
      return height_;
    }

  protected:
    /// @param pixels RGBA, width * height * 4 bytes. A taller texture holds several frames, one
    ///               per vertical slice of height.
    BaseImage(std::uint32_t width, std::uint32_t height, std::vector<std::uint8_t> pixels);

  private:
    const std::uint32_t width_;
    const std::uint32_t height_;
    std::vector<std::uint8_t> pixels_;
    d3d12::Texture texture_;
  };
}
