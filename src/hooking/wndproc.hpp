//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_WNDPROC_HPP
#define GTABASE_WNDPROC_HPP
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace gta_base {
  namespace hooking {
    bool HookWndProc();
    bool UnhookWndProc();

    LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
  }
}
#endif //GTABASE_WNDPROC_HPP
