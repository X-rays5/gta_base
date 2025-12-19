//
// Created by X-ray on 30/05/2025.
//

#include "wndproc.hpp"

namespace base::menu::hooking {
  namespace {
    LRESULT WndProcDetour(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      auto* instance = kWNDPROC;

      if (instance && globals::kRUNNING) {
        instance->HandleWndProc(hwnd, msg, wparam, lparam);
      }

      // Call the original WndProc
      if (instance) {
        auto* hook = instance->GetHook();
        if (hook) {
          return hook->CallOriginal<WNDPROC>(hwnd, msg, wparam, lparam);
        }
      }

      // Fallback if the instance is null (shouldn't happen during normal operation)
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
  }

  WndProc::WndProc() {
    auto hwnd = win32::GetGameHwnd();
    if (!hwnd) {
      LOG_ERROR("Failed to get game window handle: {}", hwnd.error());
      exit(1);
    }

    // Get the current WndProc address using GetWindowLongPtr
    auto wndproc_addr = reinterpret_cast<WNDPROC>(GetWindowLongPtrW(hwnd.value(), GWLP_WNDPROC));
    if (!wndproc_addr) {
      LOG_ERROR("Failed to get WndProc address: {}", win32::GetLastErrorStr());
      exit(1);
    }

    LOG_DEBUG("Original WndProc at: 0x{:X}", reinterpret_cast<LONG_PTR>(wndproc_addr));

    // Set the global before creating the hook so the detour can use it
    kWNDPROC = this;

    // Create and enable the detour hook
    wndproc_hook_ = std::make_unique<DetourHook>("WndProc", reinterpret_cast<void*>(wndproc_addr), reinterpret_cast<void*>(&WndProcDetour));
    wndproc_hook_->Enable();
  }

  WndProc::~WndProc() {
    LOG_DEBUG("Disabling WndProc hook...");

    // Disable the detour - PolyHook handles the cleanup safely
    if (wndproc_hook_) {
      wndproc_hook_->Disable();
    }

    // Clear the global
    kWNDPROC = nullptr;

    LOG_DEBUG("WndProc hook disabled successfully");
  }

  void WndProc::HandleWndProc(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    for (auto&& handler : wnd_proc_handlers_ | std::views::values) {
      if (handler) {
        handler(hwnd, msg, wparam, lparam);
      }
    }
  }
}