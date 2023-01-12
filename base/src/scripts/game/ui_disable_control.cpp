//
// Created by X-ray on 08/26/22.
//

#include "ui_disable_control.hpp"
#include "../../ui/manager.hpp"

namespace gta_base::scripts {

  void UIDisablePhone::Init() {

  }

  void UIDisablePhone::RunTick() {
    if (ui::kMANAGER->show_ui) {
      PAD::DISABLE_CONTROL_ACTION(0, 27, true); // disable phone
      PAD::DISABLE_CONTROL_ACTION(0, 288, true); // INPUT_REPLAY_START_STOP_RECORDING
    }
    if (ui::kMANAGER->GetBlockInputCount()) {
      PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
      PAD::DISABLE_ALL_CONTROL_ACTIONS(1);
      PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
    }
  }
}