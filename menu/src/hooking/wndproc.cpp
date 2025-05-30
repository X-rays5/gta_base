//
// Created by X-ray on 30/05/2025.
//

#include "wndproc.hpp"

namespace base::menu::hooking {
  namespace {
    LRESULT WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      if (!kWNDPROC) {
        LOG_DEBUG("kWNDPROC is null");
        return DefWindowProc(hwnd, msg, wparam, lparam);
      }

      if (globals::kRUNNING) {
        kWNDPROC->HandleWndProc(hwnd, msg, wparam, lparam);
      }

      return CallWindowProcA(kWNDPROC->GetOriginalWndProc(), hwnd, msg, wparam, lparam);
    }
  }

  WndProc::WndProc() {
    auto hwnd = win32::GetGameHwnd();
    if (!hwnd) {
      LOG_ERROR("Failed to get game window handle: {}", hwnd.error());
      exit(1);
    }

    og_wnd_proc_ = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd.value(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProcHandler)));
    LOG_DEBUG("Old WndProc handler: {}", fmt::ptr(og_wnd_proc_));

    kWNDPROC = this;
  }

  WndProc::~WndProc() {
    kWNDPROC = nullptr;

    SetWindowLongPtr(win32::GetGameHwnd().value(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(og_wnd_proc_));
  }

  void WndProc::HandleWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    for (auto&& handler : wnd_proc_handlers_ | std::views::values) {
      if (handler) {
        handler(hwnd, msg, wparam, lparam);
      }
    }
  }
}
