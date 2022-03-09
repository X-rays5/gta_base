//
// Created by X-ray on 3/9/2022.
//

#include "wndproc.hpp"

namespace ui {
  namespace windows {
    LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
      return DefWindowProc(window, message, parameter_uint_ptr, parameter_long_ptr);
    }
  }
}