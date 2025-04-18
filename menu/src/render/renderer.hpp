//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include "draw.hpp"
#include "thread.hpp"
#include <imfont/imfont.hpp>
#include <wrl/client.h>

namespace base::menu::render {
  class Renderer {
  public:
    using device_ptr_t = Microsoft::WRL::ComPtr<ID3D11Device>;
    using device_context_ptr_t = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
    using swapchain_ptr_t = Microsoft::WRL::ComPtr<IDXGISwapChain>;

  public:
    Renderer();
    ~Renderer();

    /// @note This pointer can die at any time, so don't store it.
    DrawQueueBuffer* GetDrawQueueBuffer() {
      return &draw_queue_buffer_;
    }

    device_ptr_t GetDevice() const {
      return device_;
    }

    device_context_ptr_t GetDeviceCtx() const {
      return device_ctx_;
    }

    swapchain_ptr_t GetSwapchain() const {
      return swap_chain_;
    }

    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

  private:
    device_ptr_t device_{};
    device_context_ptr_t device_ctx_{};
    swapchain_ptr_t swap_chain_{};
    DrawQueueBuffer draw_queue_buffer_;
    std::unique_ptr<imfont::Manager> font_mgr_inst_;
  };

  // skipcq: CXX-W2009
  inline Renderer* kRENDERER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
