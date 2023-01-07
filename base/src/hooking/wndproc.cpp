//
// Created by X-ray on 3/6/2022.
//

#include "wndproc.hpp"
#include "../d3d/renderer.hpp"
#include "../misc/hotkey_manager.hpp"

namespace gta_base::hooking {
WNDPROC ORIGINAL_WNDPROC = nullptr;
bool HookWndProc() {
  if (ORIGINAL_WNDPROC)
    throw std::runtime_error("WndProc already hooked");

  ORIGINAL_WNDPROC = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(common::GetHwnd(globals::target_window_class, globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));

  return ORIGINAL_WNDPROC;
}

bool UnhookWndProc() {
  if (!ORIGINAL_WNDPROC)
    throw std::runtime_error("WndProc is not hooked");

  LONG_PTR success = SetWindowLongPtrW(common::GetHwnd(globals::target_window_class, globals::target_window_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ORIGINAL_WNDPROC));

  return success;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
  if (!ORIGINAL_WNDPROC) {
    return 0;
  }

  if (globals::running) {
    switch (message) {
      case WM_SYSKEYDOWN:
      case WM_KEYDOWN: {
        common::SetKeyState(parameter_uint_ptr, true);
        break;
      }
      case WM_SYSKEYUP:
      case WM_KEYUP: {
        common::SetKeyState(parameter_uint_ptr, false);

        if (!misc::kHOTKEY_MANAGER->AddKeyPressed(parameter_uint_ptr))
          misc::kHOTKEY_MANAGER->KeyPressed(parameter_uint_ptr);
        break;
      }

      default: {
        break;
      }
    }

    d3d::Renderer::WndProc(window, message, parameter_uint_ptr, parameter_long_ptr);
  }

  return CallWindowProcA(ORIGINAL_WNDPROC, window, message, parameter_uint_ptr, parameter_long_ptr);
}
}