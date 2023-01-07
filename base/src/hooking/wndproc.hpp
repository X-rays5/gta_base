//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_WNDPROC_HPP
#define GTABASE_WNDPROC_HPP

namespace gta_base::hooking {
bool HookWndProc();
bool UnhookWndProc();

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
}
#endif //GTABASE_WNDPROC_HPP
