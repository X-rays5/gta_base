//
// Created by X-ray on 07/02/22.
//

#include "timedinput.hpp"
#include "imgui.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "util.hpp"

namespace ui {
  namespace util {
    bool TimedInput::Get() { // TODO: only when foreground window
      if ((UnixEpoch() - last_input_) > timeout_) {
        if (GetAsyncKeyState(input_)){
          last_input_ = UnixEpoch();
          return true;
        }
      }
      return false;
    }
  }
}
