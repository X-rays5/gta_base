//
// Created by X-ray on 3/6/2022.
//

#include "wndproc.hpp"
#include "../d3d/renderer.hpp"
#include "../misc/hotkey_manager.hpp"

namespace gta_base::hooking {
  WndProcHook::WndProcHook() {
    og_wndproc_ = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(common::GetHwnd(globals::target_window_class, globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook::WndProc)));

    kWND_PROC_HOOK = this;
  }

  WndProcHook::~WndProcHook() {
    kWND_PROC_HOOK = nullptr;

    LONG_PTR success = SetWindowLongPtrW(common::GetHwnd(globals::target_window_class, globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(og_wndproc_));
    LOG_ERROR_CONDITIONAL(!success, "Failed to restore original WndProc: {}", success);
  }

  LRESULT CALLBACK WndProcHook::WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
    if (!kWND_PROC_HOOK->og_wndproc_) {
      return 0;
    }

    if (globals::running) {
      kWND_PROC_HOOK->spinlock_.Lock();
      for (auto&& handler : kWND_PROC_HOOK->wnd_proc_handlers_) {
        handler(window, message, parameter_uint_ptr, parameter_long_ptr);
      }
      kWND_PROC_HOOK->spinlock_.Unlock();
    }

    return CallWindowProcA(kWND_PROC_HOOK->og_wndproc_, window, message, parameter_uint_ptr, parameter_long_ptr);
  }
}