//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include "draw.hpp"
#include "thread.hpp"

namespace base::render {
  class Renderer {
    public:
      Renderer();
      ~Renderer();

      /// @note This pointer can die at any time, so don't store it.
      DrawQueueBuffer* GetDrawQueueBuffer() { return &draw_queue_buffer_; }

      ID3D11Device* GetDevice() const { return device_; }
      ID3D11DeviceContext* GetDeviceCtx() const { return device_ctx_; }
      IDXGISwapChain* GetSwapchain() const { return swap_chain_; }

      static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
      static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

    private:
      ID3D11Device* device_{};
      ID3D11DeviceContext* device_ctx_{};
      IDXGISwapChain* swap_chain_{};
      DrawQueueBuffer draw_queue_buffer_;
  };

  inline Renderer* kRENDERER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
