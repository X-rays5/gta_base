//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_D3D11_HPP
#define GTA_BASE_D3D11_HPP
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>

class D3D11 {
public:
  D3D11(const D3D11&) = delete;
  D3D11& operator=(const D3D11&) = delete;

  static D3D11& Get() {
    static D3D11 instance;
    return instance;
  }

  static bool CreateDeviceD3D(HWND hWnd) {
    return Get().CreateDeviceD3DImpl(hWnd);
  }

  static void CleanupDeviceD3D() {
    Get().CleanupDeviceD3DImpl();
  }

  static void CreateRenderTarget() {
    Get().CreateRenderTargetImpl();
  }

  static void CleanupRenderTarget() {
    Get().CleanupRenderTargetImpl();
  }

  static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
    return Get().WndProcImpl(window, message, parameter_uint_ptr, parameter_long_ptr);
  }

  static ID3D11Device* GetDevice() {
    return Get().device_;
  }

  static ID3D11DeviceContext* GetDeviceContext() {
    return Get().device_context_;
  }

  static IDXGISwapChain* GetSwapChain() {
    return Get().swapchain_;
  }

  static ID3D11RenderTargetView* GetRenderTargetView() {
    return Get().render_target_view_;
  }
private:
  ID3D11Device*            device_ = nullptr;
  ID3D11DeviceContext*     device_context_ = nullptr;
  IDXGISwapChain*          swapchain_ = nullptr;
  ID3D11RenderTargetView*  render_target_view_ = nullptr;

private:
  D3D11() = default;

  bool CreateDeviceD3DImpl(HWND hWnd);
  void CleanupDeviceD3DImpl();
  void CreateRenderTargetImpl();
  void CleanupRenderTargetImpl();
  LRESULT CALLBACK WndProcImpl(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
};
#endif //GTA_BASE_D3D11_HPP
