//
// Created by X-ray on 09/09/22.
//

#include "keyboard.hpp"
#include "../../ui/components/keyboard.hpp"

namespace gta_base {
  namespace scripts {
    void Keyboard::Init() {
      initialized_ = true;
    }

    void Keyboard::RunTick() {
      ui::components::keyboard::kMANAGER->Tick();
    }
  }
}