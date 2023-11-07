//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include "draw.hpp"
#include <functional>
#include "thread.hpp"

namespace base::render {
  class Manager {
  public:
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

  private:
    DrawQueueBuffer draw_queue_buffer_;
    std::unique_ptr<Thread> render_thread_;
    void* present_og_;
    void* resize_buffers_og_;

  private:

  };

  inline Manager* kMANAGER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
