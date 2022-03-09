//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_WNDPROC_HPP
#define GTA_BASE_WNDPROC_HPP
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
#endif //GTA_BASE_WNDPROC_HPP
