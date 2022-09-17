//
// Created by X-ray on 09/09/22.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_SCRIPT_HPP
#define GTA_BASE_KEYBOARD_SCRIPT_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class Keyboard : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kScripting;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTA_BASE_KEYBOARD_SCRIPT_HPP
