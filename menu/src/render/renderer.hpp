//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include <dxgi.h>
#include <base-common/util/time.hpp>
#include <imfont/imfont.hpp>
#include "render_thread.hpp"
#include "../hooking/wndproc.hpp"
#include "../util/startup_shutdown_handler.hpp"
#include "../util/input/mouse_input_listener.hpp"
#include "d3d12/context.hpp"
#include "draw/draw_queue.hpp"

namespace base::menu::render {
  class Renderer final {
  public:
    Renderer();
    ~Renderer();

    /// @note This pointer can die at any time, so don't store it.
    DrawQueueBuffer* GetDrawQueueBuffer() {
      return &draw_queue_buffer_;
    }

    [[nodiscard]] std::uint64_t GetDeltaTime() const {
      return delta_time_;
    }

    [[nodiscard]] std::uint64_t GetLastTime() const {
      return last_time_;
    }

    [[nodiscard]] ImVec2 GetResolution() const {
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      return {static_cast<float>(window_width_), static_cast<float>(window_height_)};
    }

    void SetResolution(const int width, const int height) {
      LOG_INFO("Setting resolution to {}x{}", width, height);
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      window_width_ = static_cast<std::uint32_t>(width);
      window_height_ = static_cast<std::uint32_t>(height);
    }

    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

    /// Request to show cursor (reference counted)
    void RequestShowCursor();

    /// Request to hide cursor (reference counted)
    void RequestHideCursor();

    /// Get mouse state from ImGui input processing
    const ImGuiIO& GetMouseState() const {
      return ImGui::GetIO();
    }

    /// Register a mouse input listener
    void RegisterMouseInputListener(util::input::MouseInputListener* listener);

    /// Unregister a mouse input listener
    void UnregisterMouseInputListener(util::input::MouseInputListener* listener);

    static void RendererLifeTime(std::unique_ptr<Renderer>& renderer_inst, util::StartupShutdownHandler* startup_shutdown_handler) {
      startup_shutdown_handler->AddCallback("Renderer", [&](const util::StartupShutdownHandler::Action action) {
        if (action == util::StartupShutdownHandler::Action::Init) {
          renderer_inst = std::make_unique<Renderer>();
          renderer_inst->render_thread_ = std::make_unique<RenderThread>();
        } else {
          // First, make sure it's actually waiting, then unblock it.
          std::this_thread::sleep_for(std::chrono::milliseconds(200));
          renderer_inst->GetDrawQueueBuffer()->UnblockRenderThread();

          renderer_inst.reset();
        }
      });
    }

  private:
    mutable common::concurrency::Spinlock window_size_lock_;
    bool init_imgui_ = false;
    std::uint32_t window_width_{};
    std::uint32_t window_height_{};
    std::atomic<std::int32_t> cursor_show_requests_ = 0;
    DrawQueueBuffer draw_queue_buffer_;
    std::unique_ptr<imfont::Manager> font_mgr_inst_;
    std::uint64_t delta_time_ = 0;
    std::uint64_t last_time_ = common::util::time::GetTimeStamp();
    std::size_t wndproc_handler_id_{};
    std::unique_ptr<RenderThread> render_thread_;
    d3d12::Context d3d12_context_;
    std::vector<util::input::MouseInputListener*> mouse_input_listeners_;

  private:
    void SetDeltaTime(const std::uint64_t delta_time) {
      delta_time_ = delta_time;
    }

    void SetLastTime(const std::uint64_t last_time) {
      last_time_ = last_time;
    }

    void InitImGui();
  };

  inline Renderer* kRENDERER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
