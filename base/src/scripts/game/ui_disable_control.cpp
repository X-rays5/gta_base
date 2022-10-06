//
// Created by X-ray on 08/26/22.
//

#include "ui_disable_control.hpp"
#include "../../natives/natives.hpp"
#include "../../ui/manager.hpp"

namespace gta_base {
  bool globals::block_input = false;

  namespace scripts {

    void UIDisablePhone::Init() {

    }

    void UIDisablePhone::RunTick() {
      if (ui::kMANAGER->show_ui) {
        PAD::DISABLE_CONTROL_ACTION(0, 27, true); // disable phone
        PAD::DISABLE_CONTROL_ACTION(0, 288, true); // INPUT_REPLAY_START_STOP_RECORDING
      }
      if (globals::block_input) {
        PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
        PAD::DISABLE_ALL_CONTROL_ACTIONS(1);
        PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
      }
    }
  }
}