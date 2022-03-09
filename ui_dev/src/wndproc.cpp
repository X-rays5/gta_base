//
// Created by X-ray on 3/9/2022.
//

#include "wndproc.hpp"
#include "ui/ui.hpp"
#include "d3d11/d3d11.hpp"

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
  ui::WndProc(window, message, parameter_uint_ptr, parameter_long_ptr);
  D3D11::WndProc(window, message, parameter_uint_ptr, parameter_long_ptr);

  return DefWindowProc(window, message, parameter_uint_ptr, parameter_long_ptr);
}