//
// Created by X-ray on 24/03/2026.
//

#include "feature_loop.hpp"
#include "../natives/natives_gen9.hpp"
#include "../options/option_registry.hpp"
#include "../render/renderer.hpp"
#include "../ui/menu_renderer.hpp"

namespace base::menu::feature {
  namespace {
    thread_local natives::Player player_id;
    thread_local natives::Ped player_ped;
    thread_local bool is_player_in_vehicle;
    thread_local natives::Vehicle player_vehicle;

    void DisableGameUIInteractWhenMenuOpened() {
      if (ui::kMENU_RENDERER->IsMenuOpened()) {
        natives::PAD::DISABLE_CONTROL_ACTION(0, 18, true); // INPUT_SKIP_CUTSCENE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 27, true); // INPUT_PHONE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 172, true); // INPUT_CELLPHONE_UP
        natives::PAD::DISABLE_CONTROL_ACTION(0, 173, true); // INPUT_CELLPHONE_DOWN
        natives::PAD::DISABLE_CONTROL_ACTION(0, 174, true); // INPUT_CELLPHONE_LEFT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 175, true); // INPUT_CELLPHONE_RIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 176, true); // INPUT_CELLPHONE_SELECT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 177, true); // INPUT_CELLPHONE_CANCEL
        natives::PAD::DISABLE_CONTROL_ACTION(2, 187, true); // INPUT_FRONTEND_DOWN
        natives::PAD::DISABLE_CONTROL_ACTION(2, 188, true); // INPUT_FRONTEND_UP
        natives::PAD::DISABLE_CONTROL_ACTION(2, 189, true); // INPUT_FRONTEND_LEFT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 190, true); // INPUT_FRONTEND_RIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 191, true); // INPUT_FRONTEND_RDOWN
        natives::PAD::DISABLE_CONTROL_ACTION(2, 194, true); // INPUT_FRONTEND_RRIGHT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 201, true); // INPUT_FRONTEND_ACCEPT
        natives::PAD::DISABLE_CONTROL_ACTION(2, 202, true); // INPUT_FRONTEND_CANCEL
        natives::PAD::DISABLE_CONTROL_ACTION(2, 215, true); // INPUT_FRONTEND_ENDSCREEN_ACCEPT
        natives::PAD::DISABLE_CONTROL_ACTION(0, 299, true); // INPUT_REPLAY_REWIND
        natives::PAD::DISABLE_CONTROL_ACTION(0, 300, true); // INPUT_REPLAY_FFWD
        natives::PAD::DISABLE_CONTROL_ACTION(0, 307, true); // INPUT_REPLAY_ADVANCE
        natives::PAD::DISABLE_CONTROL_ACTION(0, 308, true); // INPUT_REPLAY_BACK
      }
    }
  }

  GameFeatureLoop::GameFeatureLoop() : ScriptBase{"GameFeatureLoop"} {}

  void GameFeatureLoop::OnInit() {
    LOG_INFO("GameFeatureLoop initialized.");
  }

  void GameFeatureLoop::OnTick() {
    player_id = natives::PLAYER::PLAYER_ID();
    player_ped = natives::PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player_id);
    is_player_in_vehicle = natives::PED::IS_PED_IN_ANY_VEHICLE(player_ped, false);
    if (is_player_in_vehicle) {
      player_vehicle = natives::PED::GET_VEHICLE_PED_IS_IN(player_ped, false);
    } else {
      player_vehicle = 0;
    }

    if (ui::kMENU_RENDERER) {
      DisableGameUIInteractWhenMenuOpened();
    }

    if (render::kRENDERER && render::kRENDERER->IsCursorVisible()) {
      natives::PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
    }

    if (options::kOPTION_REGISTRY) {
      const auto all_opt = options::kOPTION_REGISTRY->GetAllOptions();
      for (auto&& opt : all_opt) {
        if (opt && opt->IsTickable() && opt->GetTickThread() == options::BaseOption::TickThread::kGAME_SCRIPT) {
          opt->Tick();
        }
      }
    }
  }
}
