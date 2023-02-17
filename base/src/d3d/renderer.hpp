//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_RENDERER_HPP
#define GTA_BASE_RENDERER_HPP
#include <memory>
#include <imgui.h>
#include <wrl/client.h>
#include <d3d11.h>

namespace gta_base::d3d {
  class Renderer {
  public:
    explicit Renderer(HWND hwnd);
    ~Renderer();

    inline Microsoft::WRL::ComPtr <ID3D11Device> GetDevice() {
      return device_;
    }

    inline Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetDeviceContext() {
      return device_context_;
    }

    inline Microsoft::WRL::ComPtr <IDXGISwapChain> GetSwapChain() {
      return swap_chain_;
    }

    inline HWND GetHwnd() {
      return hwnd_;
    }

    inline ImFont* GetFont() {
      return roboto_;
    }

    inline ImFont* GetFontBold() {
      return roboto_bold_;
    }

    [[nodiscard]] inline std::uint64_t GetDeltaTime() const {
      return delta_time_;
    }

    inline void SetDeltaTime(std::uint64_t delta_time) {
      delta_time_ = delta_time;
    }

    [[nodiscard]] inline std::uint64_t GetLastTime() const {
      return last_time_;
    }

    inline void SetLastTime(std::uint64_t last_time) {
      last_time_ = last_time;
    }

    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT SwapChainResizeBuffer(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
  private:
    Microsoft::WRL::ComPtr <IDXGISwapChain> swap_chain_;
    Microsoft::WRL::ComPtr <ID3D11Device> device_;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context_;
    HWND hwnd_;
    ImFont* roboto_ = nullptr;
    ImFont* roboto_bold_ = nullptr;
    std::uint64_t delta_time_{};
    std::uint64_t last_time_{};

  private:
    void InitD3D();
  };

  inline Renderer* kRENDERER{};
}

#endif //GTA_BASE_RENDERER_HPP
