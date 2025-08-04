//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_RENDERER_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_RENDERER_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include <imfont/imfont.hpp>
#include <wrl/client.h>
#include "render_thread.hpp"
#include "../hooking/wndproc.hpp"
#include "../util/startup_shutdown_handler.hpp"
#include "draw/draw_queue.hpp"

namespace base::menu::render {
  class Renderer final : public hooking::WndProcEventListener {
  public:
    using device_ptr_t = Microsoft::WRL::ComPtr<ID3D11Device>;
    using device_context_ptr_t = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
    using swapchain_ptr_t = Microsoft::WRL::ComPtr<IDXGISwapChain>;

  public:
    Renderer();
    virtual ~Renderer() override;

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

    [[nodiscard]] std::uint64_t GetDeltaTime() const {
      return delta_time_;
    }

    [[nodiscard]] std::uint64_t GetLastTime() const {
      return last_time_;
    }

    ImVec2 GetResolution() {
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      return {static_cast<float>(window_width_), static_cast<float>(window_height_)};
    }

    void SetResolution(const int width, const int height) {
      LOG_INFO("Setting resolution to {}x{}", width, height);
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      window_width_ = static_cast<std::uint32_t>(width);
      window_height_ = static_cast<std::uint32_t>(height);
    }

    virtual void OnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

  private:
    device_ptr_t device_{};
    device_context_ptr_t device_ctx_{};
    swapchain_ptr_t swap_chain_{};
    DrawQueueBuffer draw_queue_buffer_;
    std::unique_ptr<imfont::Manager> font_mgr_inst_;
    std::uint64_t delta_time_ = 0;
    std::uint64_t last_time_ = common::util::time::GetTimeStamp();
    std::unique_ptr<RenderThread> render_thread_;
    bool init_imgui_ = false;

  private:
    void SetDeltaTime(const std::uint64_t delta_time) {
      delta_time_ = delta_time;
    }

    void SetLastTime(const std::uint64_t last_time) {
      last_time_ = last_time;
    }

    std::size_t wndproc_handler_id_{};

    common::concurrency::Spinlock window_size_lock_;
    std::uint32_t window_width_{};
    std::uint32_t window_height_{};
  };

  inline Renderer* kRENDERER{};

  inline void RendererLifeTime(std::unique_ptr<Renderer>& renderer_inst, util::StartupShutdownHandler* startup_shutdown_handler) {
    startup_shutdown_handler->AddCallback("Renderer", [&](const util::StartupShutdownHandler::Action action) {
      if (action == util::StartupShutdownHandler::Action::Init) {
        renderer_inst = std::make_unique<Renderer>();
      } else {
        // First, make sure it's actually waiting, then unblock it.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        renderer_inst->GetDrawQueueBuffer()->UnblockRenderThread();

        renderer_inst.reset();
      }
    });
  }
}
#endif //BASE_RENDERER_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
