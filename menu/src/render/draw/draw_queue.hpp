//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#define BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#include <memory>
#include <vector>
#include <base-common/concurrency/spinlock.hpp>
#include "draw_commands.hpp"

namespace base::menu::render {
  class DrawQueue {
  public:
    template <typename T> requires std::is_base_of_v<BaseDrawCommand, T>
    FORCE_INLINE
    void AddCommand(T&& command) {
      draw_commands_.push_back(std::make_unique<T>(std::forward<T>(command)));
    }

    FORCE_INLINE void Clear() {
      draw_commands_.clear();
    }

    FORCE_INLINE void Draw() const {
      for (auto&& command : draw_commands_) {
        command->Draw();
      }
    }

  private:
    std::vector<std::unique_ptr<BaseDrawCommand>> draw_commands_;
  };

  class DrawQueueBuffer {
  public:
    explicit DrawQueueBuffer(std::size_t draw_queue_buffers = 3);
    ~DrawQueueBuffer();

    /// @note This function is only supposed to be called when shutting down to prevent a deadlock. In the case the render thread was waiting for a signal, but the render detour is already disabled.
    FORCE_INLINE void UnblockRenderThread() const {
      read_signal_.Notify();
    }

    void RenderFrame();

    /// @note This function should ONLY be called from the render thread.
    template <typename... Args>
    FORCE_INLINE void AddCommand(Args&&... command) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      draw_queue_[write_idx_].AddCommand(std::forward<Args>(command)...);
    }

    /// @warning This function may ONLY be called from RenderThread::RenderMain().
    void SwapBuffers();

  private:
    std::vector<DrawQueue> draw_queue_;
    std::size_t read_idx_ = 0;
    std::size_t write_idx_ = 1;
    common::concurrency::Spinlock spinlock_;
    win32::Signal read_signal_;
  };
}
#endif //BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
