//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include "draw.hpp"
#include <functional>

namespace base::render {
  class Manager {
  public:
    using render_cb_t = std::function<void()>;

    Manager();
    ~Manager();

    /// @note This pointer can die at any time, so don't store it.
    inline DrawQueueBuffer* GetDrawQueueBuffer() {
      return &draw_queue_buffer_;
    }

    inline void* GetPresentOg() {
      return present_og_;
    }

    inline void* GetResizeBuffersOg() {
      return resize_buffers_og_;
    }

    inline void AddRenderCallback(const render_cb_t& cb) {
      render_callbacks_.push_back(cb);
    }

    inline std::vector<render_cb_t>& GetRenderCallbacks() {
      return render_callbacks_;
    }

  private:
    std::vector<render_cb_t> render_callbacks_;
    DrawQueueBuffer draw_queue_buffer_;
    void* present_og_;
    void* resize_buffers_og_;

  private:

  };

  inline Manager* kMANAGER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
