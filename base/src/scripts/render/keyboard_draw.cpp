//
// Created by X-ray on 09/09/22.
//

#include "keyboard_draw.hpp"
#include "../../ui/components/keyboard.hpp"

namespace gta_base {
  namespace scripts {
    void KeyboardDraw::Init() {
      initialized_ = true;
    }

    void KeyboardDraw::RunTick() {
      ui::components::keyboard::kMANAGER->Tick();
    }
  }
}