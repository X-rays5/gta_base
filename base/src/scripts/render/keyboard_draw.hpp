//
// Created by X-ray on 09/09/22.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_DRAW_HPP
#define GTA_BASE_KEYBOARD_DRAW_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class KeyboardDraw : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kRenderer;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTA_BASE_KEYBOARD_DRAW_HPP
