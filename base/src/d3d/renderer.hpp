//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_RENDERER_HPP
#define GTA_BASE_RENDERER_HPP
#include <memory>
#include <imgui.h>
#define win32_lean_and_mean
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "texturemanager.hpp"
#include "draw.hpp"

namespace gta_base {
  namespace d3d {
    class Renderer {
    public:
      explicit Renderer(HWND hwnd);
      ~Renderer();

      inline Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() {
        return device_;
      }

      inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() {
        return device_context_;
      }

      inline Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() {
        return swap_chain_;
      }

      inline HWND GetHwnd() {
        return hwnd_;
      }

      inline ImFont* GetFont() {
        return roboto_;
      }

      static void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

      static HRESULT Present(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags);
      static HRESULT SwapChainResizeBuffer(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
    private:
      std::unique_ptr<TextureManager> texture_manager_;

      Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_;
      Microsoft::WRL::ComPtr<ID3D11Device> device_;
      Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context_;
      HWND hwnd_;
      ImFont* roboto_ = nullptr;

    private:
      void InitD3D();
    };
    inline Renderer* kRENDERER{};
  }
}

#endif //GTA_BASE_RENDERER_HPP
