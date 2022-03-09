//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "d3d11.hpp"

#pragma comment(lib, "d3d11.lib")

bool D3D11::CreateDeviceD3DImpl(HWND hWnd) {
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
  auto create_device_spawnchain_res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swapchain_, &device_, &featureLevel, &device_context_);
  if (create_device_spawnchain_res != S_OK) {
    std::cerr << "Failed to create device and swapchain" << '\n' << create_device_spawnchain_res << std::endl;
    return false;
  }

  CreateRenderTarget();
  return true;
}

void D3D11::CleanupDeviceD3DImpl() {
  CleanupRenderTarget();
  if (swapchain_) { swapchain_->Release(); swapchain_ = nullptr; }
  if (device_context_) { device_context_->Release(); device_context_ = nullptr; }
  if (device_) { device_->Release(); device_ = nullptr; }
}

void D3D11::CreateRenderTargetImpl() {
  ID3D11Texture2D* pBackBuffer;
  swapchain_->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  device_->CreateRenderTargetView(pBackBuffer, nullptr, &render_target_view_);
  pBackBuffer->Release();
}

void D3D11::CleanupRenderTargetImpl() {
  if (render_target_view_) { render_target_view_->Release(); render_target_view_ = nullptr; }
}

LRESULT D3D11::WndProcImpl(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
  switch (message) {
    case WM_SIZE:
      if (device_ != nullptr && parameter_uint_ptr != SIZE_MINIMIZED)
      {
        CleanupRenderTarget();
        swapchain_->ResizeBuffers(0, (UINT)LOWORD(parameter_long_ptr), (UINT)HIWORD(parameter_long_ptr), DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
      }
      return 0;
    case WM_SYSCOMMAND:
      if ((parameter_uint_ptr & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
        return 0;
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
  }
  return 0;
}
