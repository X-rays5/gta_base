//
// Created by X-ray on 24/03/2026.
//

#include "feature_loop.hpp"
#include "../natives/natives_gen9.hpp"
#include "../ui/menu_renderer.hpp"

namespace base::menu::feature {
  namespace {
    void DisableGameUIInteractWhenMenuOpened() {
      if (ui::kMENU_RENDERER->IsMenuOpened()) {
        natives::PAD::DISABLE_CONTROL_ACTION(0, 18, false); // INPUT_SKIP_CUTSCENE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 27, false); // INPUT_PHONE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 172, false); // INPUT_CELLPHONE_UP
        natives::PAD::DISABLE_CONTROL_ACTION(0, 173, false); // INPUT_CELLPHONE_DOWN
        natives::PAD::DISABLE_CONTROL_ACTION(0, 174, false); // INPUT_CELLPHONE_LEFT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 175, false); // INPUT_CELLPHONE_RIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 176, false); // INPUT_CELLPHONE_SELECT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 177, false); // INPUT_CELLPHONE_CANCEL
        natives::PAD::DISABLE_CONTROL_ACTION(2, 187, false); // INPUT_FRONTEND_DOWN
        natives::PAD::DISABLE_CONTROL_ACTION(2, 188, false); // INPUT_FRONTEND_UP
        natives::PAD::DISABLE_CONTROL_ACTION(2, 189, false); // INPUT_FRONTEND_LEFT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 190, false); // INPUT_FRONTEND_RIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 191, false); // INPUT_FRONTEND_RDOWN
        natives::PAD::DISABLE_CONTROL_ACTION(2, 194, false); // INPUT_FRONTEND_RRIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 201, false); // INPUT_FRONTEND_ACCEPT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 202, false); // INPUT_FRONTEND_CANCEL
        natives::PAD::DISABLE_CONTROL_ACTION(2, 215, false); // INPUT_FRONTEND_ENDSCREEN_ACCEPT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 299, false); // INPUT_REPLAY_REWIND
        natives::PAD::DISABLE_CONTROL_ACTION(0, 300, false); // INPUT_REPLAY_FFWD
        natives::PAD::DISABLE_CONTROL_ACTION(0, 307, false); // INPUT_REPLAY_ADVANCE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 308, false); // INPUT_REPLAY_BACK
      }
    }
  }

  GameFeatureLoop::GameFeatureLoop() : ScriptBase{"GameFeatureLoop"} {}

  void GameFeatureLoop::OnInit() {
    LOG_INFO("GameFeatureLoop initialized.");
  }

  void GameFeatureLoop::OnTick() {
    if (ui::kMENU_RENDERER) {
      DisableGameUIInteractWhenMenuOpened();
    }
  }
}
