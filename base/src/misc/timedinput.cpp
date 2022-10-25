//
// Created by X-ray on 07/02/22.
//

#include "timedinput.hpp"
#include "common.hpp"

namespace gta_base::util {
    bool TimedInput::Get() {
      if ((common::GetEpoch() - last_input_) > timeout_) {
        if (common::IsKeyDown(input_)){
          last_input_ = common::GetEpoch();
          return true;
        }
      }
      return false;
    }

  bool ModifierTimedInput::Get() {
    if ((common::GetEpoch() - last_input_) > timeout_) {
      if (common::IsKeyDown(modifier_) && common::IsKeyDown(input_)){
        last_input_ = common::GetEpoch();
        return true;
      }
    }

    return false;
  }
}