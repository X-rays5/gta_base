//
// Created by X-ray on 3/6/2022.
//

#include "wndproc.hpp"
#include "../common/common.hpp"
#include "../logger/logger.hpp"

namespace gta_base {
  namespace hooking {
    WNDPROC ORIGINAL_WNDPROC = nullptr;
    bool HookWndProc() {
      if (ORIGINAL_WNDPROC)
        throw std::runtime_error("WndProc already hooked");

      ORIGINAL_WNDPROC = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(common::GetHwnd(common::globals::target_window_class, common::globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));

      return ORIGINAL_WNDPROC;
    }

    bool UnhookWndProc() {
      if (!ORIGINAL_WNDPROC)
        throw std::runtime_error("WndProc is not hooked");

      LONG_PTR success = SetWindowLongPtrW(common::GetHwnd(common::globals::target_window_class, common::globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ORIGINAL_WNDPROC));

      return success;
    }

    LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
      if (!ORIGINAL_WNDPROC) {
        return 0;
      }

      return CallWindowProcA(ORIGINAL_WNDPROC, window, message, parameter_uint_ptr, parameter_long_ptr);
    }
  }
}