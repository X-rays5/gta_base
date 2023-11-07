//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#define BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#include <vector>
#include <memory>
#include <imgui.h>
#include <absl/synchronization/notification.h>
#include "draw_util.hpp"
#include "draw_commands.hpp"
#include "../util/spinlock.hpp"

namespace base::render {
  class DrawQueue {
  public:
    template<typename T>
    requires std::is_base_of_v<BaseDrawCommand, T>
    FORCE_INLINE void AddCommand(T command) {
      draw_commands_.push_back(std::make_unique<T>(command));
    }

    void Draw() {
      for (auto&& command : draw_commands_) {
        command->Draw();
      }
    }

    FORCE_INLINE void Clear() {
      draw_commands_.clear();
    }

  private:
    std::vector<std::unique_ptr<BaseDrawCommand>> draw_commands_;

  private:

  };

  class DrawQueueBuffer {
  public:
    DrawQueueBuffer(std::size_t draw_queue_buffers = 3) {
      GTA_BASE_ASSERT(draw_queue_buffers > 1, "Draw queue buffers must be greater than 1.");

      draw_queue_.resize(draw_queue_buffers);
      read_idx_ = 0;
      write_idx_ = 1;
      read_notification_ = std::make_unique<absl::Notification>();
    }

    ~DrawQueueBuffer() {
      // Just in case to prevent a deadlock on shutdown.
      ::base::util::ScopedSpinlock lock(spinlock_);
      read_notification_->Notify();
    }

    inline void RenderFrame() {
      ::base::util::ScopedSpinlock lock(spinlock_);
      draw_queue_[read_idx_].Draw();
      read_notification_->Notify();
    }

    /// @note This function should ONLY be called from the render thread.
    template<typename ...Args>
    inline void AddCommand(Args&& ... command) {
      draw_queue_[write_idx_].AddCommand(std::forward<Args>(command)...);
    }

    /// @warning This function may ONLY be called from Thread::RenderMain().
    void SwapBuffers() {
      spinlock_.Lock();

      read_idx_ = write_idx_;
      write_idx_ = (write_idx_ + 1) % draw_queue_.size();
      draw_queue_[write_idx_].Clear();

      spinlock_.Unlock();

      read_notification_->WaitForNotification();

      spinlock_.Lock();
      read_notification_ = std::make_unique<absl::Notification>();
      spinlock_.Unlock();
    }

  private:
    std::vector<DrawQueue> draw_queue_;
    std::size_t read_idx_;
    std::size_t write_idx_;
    ::base::util::Spinlock spinlock_;
    std::unique_ptr<absl::Notification> read_notification_;
  };
}
#endif //BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
