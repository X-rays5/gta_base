//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#define BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#include <memory>
#include <vector>
#include <base-common/concurrency/spinlock.hpp>
#include <imgui/imgui.h>
#include "draw_commands.hpp"
#include "draw_util.hpp"

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
    explicit DrawQueueBuffer(const std::size_t draw_queue_buffers = 3) {
      GTA_BASE_ASSERT(draw_queue_buffers > 1, "Draw queue buffers must be greater than 1.");

      draw_queue_.resize(draw_queue_buffers);
    }

    ~DrawQueueBuffer() {
      // Just in case to prevent a deadlock on shutdown.
      common::concurrency::ScopedSpinlock lock(spinlock_);
      read_signal_.Notify();
    }

    /// @note This function is only supposed to be called when shutting down to prevent a deadlock. In the case the render thread was waiting for a signal, but the render detour is already disabled.
    void UnblockRenderThread() const {
      read_signal_.Notify();
    }

    void RenderFrame() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      draw_queue_[read_idx_].Draw();
      read_signal_.Notify();
    }

    /// @note This function should ONLY be called from the render thread.
    template <typename... Args>
    void AddCommand(Args&&... command) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      draw_queue_[write_idx_].AddCommand(std::forward<Args>(command)...);
    }

    /// @warning This function may ONLY be called from Thread::RenderMain().
    void SwapBuffers() {
      spinlock_.Lock();

      read_idx_ = write_idx_;
      write_idx_ = (write_idx_ + 1) % draw_queue_.size();
      draw_queue_[write_idx_].Clear();

      spinlock_.Unlock();

      read_signal_.Wait();
    }

  private:
    std::vector<DrawQueue> draw_queue_;
    std::size_t read_idx_ = 0;
    std::size_t write_idx_ = 1;
    common::concurrency::Spinlock spinlock_;
    win32::Signal read_signal_;
  };
}
#endif //BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
