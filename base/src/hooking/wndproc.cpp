//
// Created by X-ray on 3/6/2022.
//

#include "wndproc.hpp"
#include "../d3d/renderer.hpp"
#include "../misc/hotkey_manager.hpp"

namespace gta_base::hooking {
  WndProcHook::WndProcHook() {
    kWND_PROC_HOOK = this;
  }

  WndProcHook::~WndProcHook() {
    kWND_PROC_HOOK = nullptr;

    Disable();
  }

  LRESULT CALLBACK WndProcHook::WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
    if (!kWND_PROC_HOOK->og_wndproc_) {
      LOG_CRITICAL("og wndproc nullptr");
      return 0;
    }

    LRESULT ret_val = CallWindowProc(kWND_PROC_HOOK->og_wndproc_, window, message, parameter_uint_ptr, parameter_long_ptr);

    if (globals::running) {
      kWND_PROC_HOOK->spinlock_.Lock();
      for (auto&& handler : kWND_PROC_HOOK->wnd_proc_handlers_) {
        handler(window, message, parameter_uint_ptr, parameter_long_ptr);
      }
      kWND_PROC_HOOK->spinlock_.Unlock();
    }

    return ret_val;
  }
}