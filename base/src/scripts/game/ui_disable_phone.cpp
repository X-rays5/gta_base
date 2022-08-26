//
// Created by X-ray on 08/26/22.
//

#include "ui_disable_phone.hpp"
#include "../../natives/natives.hpp"
#include "../../ui/manager.hpp"

namespace gta_base {
  namespace scripts {

    void UIDisablePhone::Init() {

    }

    void UIDisablePhone::RunTick() {
      if (ui::kMANAGER->show_ui)
        PAD::DISABLE_CONTROL_ACTION(0, 27, true); // disable phone
    }
  }
}