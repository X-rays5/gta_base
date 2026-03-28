//
// Thread-safe input event queue for ImGui in multi-threaded environments
//
// Problem: WndProc runs on the Windows message pump thread, ImGui::NewFrame runs on render thread.
// If we call ImGui_ImplWin32_WndProcHandler directly from WndProc, it modifies ImGui::IO state
// while render thread is calling ImGui::NewFrame(), causing data races.
//
// Solution: Queue events from WndProc (message pump thread), process them on render thread
// before ImGui::NewFrame() where it's safe (single-threaded).
//
// Why queuing instead of blocking:
// - Blocking WndProc causes input lag and OS unresponsive window detection
// - Queuing is instant (~microseconds) and non-blocking
// - Events processed same frame anyway (no noticeable delay)
//

#pragma once
#include <queue>
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::render {
  /**
   * @class ImGuiInputQueue
   * @brief Thread-safe queue for ImGui Win32 input events
   *
   * Allows WndProc (message pump thread) to queue input events without blocking.
   * Render thread processes all queued events in NewFrame() where it's safe.
   */
  class ImGuiInputQueue {
  public:
    struct InputEvent {
      HWND hwnd;
      UINT msg;
      WPARAM wparam;
      LPARAM lparam;
    };

    static ImGuiInputQueue& Get() {
      static ImGuiInputQueue instance;
      return instance;
    }

    /// Queue an input event from WndProc (non-blocking, safe from message pump thread)
    void QueueEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      common::concurrency::ScopedSpinlock lock(queue_lock_);
      event_queue_.emplace(hwnd, msg, wparam, lparam);
    }

    /// Process all queued events (called on render thread before ImGui::NewFrame())
    /// This is where it's safe to modify ImGui::IO state
    /// Implementation in imgui_input_queue.cpp to avoid linker issues
    void ProcessQueuedEvents();

    void Clear() {
      common::concurrency::ScopedSpinlock lock(queue_lock_);
      while (!event_queue_.empty()) {
        event_queue_.pop();
      }
    }

  private:
    ImGuiInputQueue() = default;
    ~ImGuiInputQueue() = default;

    std::queue<InputEvent> event_queue_;
    mutable common::concurrency::Spinlock queue_lock_;
  };
}



