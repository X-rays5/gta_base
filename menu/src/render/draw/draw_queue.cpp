//
// Created by X-ray on 31/05/2025.
//

#include "draw_queue.hpp"

namespace base::menu::render {
  DrawQueueBuffer::DrawQueueBuffer(const std::size_t draw_queue_buffers) {
    GTA_BASE_ASSERT(draw_queue_buffers > 1, "Draw queue buffers must be greater than 1.");

    draw_queue_.resize(draw_queue_buffers);
  }

  DrawQueueBuffer::~DrawQueueBuffer() {
    // Just in case to prevent a deadlock on shutdown.
    common::concurrency::ScopedSpinlock lock(spinlock_);
    read_signal_.Notify();
  }

  void DrawQueueBuffer::RenderFrame() {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    draw_queue_[read_idx_].Draw();
    UnblockRenderThread();
  }

  void DrawQueueBuffer::SwapBuffers() {
    spinlock_.Lock();

    read_idx_ = write_idx_;
    write_idx_ = (write_idx_ + 1) % draw_queue_.size();
    draw_queue_[write_idx_].Clear();

    spinlock_.Unlock();

    read_signal_.Wait();
  }
}
