//
// Created by X-ray on 02/17/23.
//

#include "cooldown_input.hpp"
#include "manager.hpp"

namespace gta_base::key_input {
  bool CooldownInput::Get() {
    if (common::GetEpoch() - last_input_ > timeout_) {
      if (kINPUT_MGR->IsControlPressed(key_bind_id_)) {
        last_input_ = common::GetEpoch();
        return true;
      }
    }

    return false;
  }
}
