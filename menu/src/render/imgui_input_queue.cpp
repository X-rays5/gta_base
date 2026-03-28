#include "imgui_input_queue.hpp"
#include <imgui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace base::menu::render {
  void ImGuiInputQueue::ProcessQueuedEvents() {
    common::concurrency::ScopedSpinlock lock(queue_lock_);
    while (!event_queue_.empty()) {
      const auto& event = event_queue_.front();
      ImGui_ImplWin32_WndProcHandler(event.hwnd, event.msg, event.wparam, event.lparam);
      event_queue_.pop();
    }
  }
}

