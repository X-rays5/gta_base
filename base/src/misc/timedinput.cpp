//
// Created by X-ray on 07/02/22.
//

#include "timedinput.hpp"
#include "common.hpp"

namespace gta_base {
  namespace util {
    bool TimedInput::Get() { // TODO: only when foreground window
      if ((common::GetEpoch() - last_input_) > timeout_) {
        if (common::KeyState(input_)){
          last_input_ = common::GetEpoch();
          return true;
        }
      }
      return false;
    }
  }
}