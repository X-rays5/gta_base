//
// Created by X-ray on 14/09/2026.
//

#pragma once
#include <span>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>

namespace base::menu::render::d3d12 {
  /**
   * A 2D RGBA texture and the SRV descriptor that points at it.
   *
   * @note Create() uploads through the renderer's command queue, so it must run on the render
   *       thread, and only once the renderer exists.
   */
  class Texture final {
  public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    /// @returns false if the upload failed, leaving the texture invalid.
    bool Create(std::span<const std::uint8_t> rgba_pixels, std::uint32_t width, std::uint32_t height);
    void Destroy();

    [[nodiscard]] bool IsValid() const {
      return resource_ != nullptr;
    }

    [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const {
      return gpu_handle_;
    }

    [[nodiscard]] std::uint32_t GetWidth() const {
      return width_;
    }

    [[nodiscard]] std::uint32_t GetHeight() const {
      return height_;
    }

  private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_{};
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_{};
    bool has_descriptor_ = false;
    std::uint32_t width_ = 0;
    std::uint32_t height_ = 0;
  };
}
