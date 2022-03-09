//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_UI_HPP
#define GTA_BASE_UI_HPP
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

namespace ui {
  bool IsInitialized();
  void Initialize();
  void Shutdown();
  void Tick();

  LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
}
#endif //GTA_BASE_UI_HPP
