//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include <dxgi.h>
#include <imfont/imfont.hpp>
#include "draw.hpp"
#include "d3d12/context.hpp"

namespace base::menu::render {
  class Renderer {
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

    ImVec2 GetResolution() {
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      return {static_cast<float>(window_width_), static_cast<float>(window_height_)};
    }

    void SetResolution(const int width, const int height) {
      common::concurrency::ScopedSpinlock lock(window_size_lock_);
      window_width_ = static_cast<std::uint32_t>(width);
      window_height_ = static_cast<std::uint32_t>(height);
    }

    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

  private:
    common::concurrency::Spinlock window_size_lock_;
    bool init_imgui_ = false;
    bool d3d12_resizing_ = false;
    std::uint32_t window_width_{};
    std::uint32_t window_height_{};
    DrawQueueBuffer draw_queue_buffer_;
    std::unique_ptr<imfont::Manager> font_mgr_inst_;
    std::uint64_t delta_time_ = 0;
    std::uint64_t last_time_ = common::util::time::GetTimeStamp();
    std::size_t wndproc_handler_id_{};
    d3d12::Context d3d12_context_;

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
