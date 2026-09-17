//
// Created by X-ray on 24/03/2026.
//

#include "feature_loop.hpp"
#include "../game/globals.hpp"
#include "../natives/natives_gen9.hpp"
#include "../options/option_registry.hpp"
#include "../render/renderer.hpp"
#include "../ui/menu_renderer.hpp"

namespace base::menu::feature {
  namespace {
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

    /// Whether the last tick had a local ped, so that the ticks where one appears or goes away are
    /// marked in the log and the sixty a second in between stay quiet.
    bool had_local_ped{};

    void UpdateGameGlobals() {
      const auto player = natives::PLAYER::PLAYER_ID();
      const auto ped = player.GetPed();

      game::globals::local_player.player_id = player;
      game::globals::local_player.ped_id = ped;

      const auto has_ped = ped.IsValid();
      if (!has_ped) {
        // The vehicle natives dereference the ped, and the first ticks can run before the game has
        // spawned one - GET_VEHICLE_PED_IS_IN on an invalid ped faults inside the game. With no local
        // ped there is no vehicle to report either, so the globals go back to zero rather than keeping
        // whatever the ped that just went away was in.
        game::globals::local_player.vehicle = natives::Vehicle{};
        game::globals::local_player.vehicle_include_entering = natives::Vehicle{};
        game::globals::local_player.last_vehicle = natives::Vehicle{};
      } else {
        // A handle is taken only when the matching in-vehicle question answers yes: GET_VEHICLE_PED_IS_IN
        // keeps naming the vehicle a ped has got out of - the game retains m_pMyVehicle past the exit and
        // clears it only for a ped carrying CPED_CONFIG_FLAG_ResetLastVehicleOnVehicleExit - while
        // CPED_CONFIG_FLAG_InVehicle, which IS_PED_IN_ANY_VEHICLE reads, is cleared on the way out. Asked
        // for the handle alone, these globals report a vehicle for a player standing on the street.
        const auto in_vehicle = ped.IsInAnyVehicle(false);
        const auto entering_vehicle = ped.IsInAnyVehicle(true);
        const auto vehicle = ped.GetVehicle(false);
        const auto entering = ped.GetVehicle(true);
        const auto last_vehicle = natives::PLAYER::GET_PLAYERS_LAST_VEHICLE();

        game::globals::local_player.vehicle = in_vehicle ? vehicle.value_or(natives::Vehicle{}) : natives::Vehicle{};
        game::globals::local_player.vehicle_include_entering = entering_vehicle ? entering.value_or(natives::Vehicle{}) : natives::Vehicle{};
        game::globals::local_player.last_vehicle = last_vehicle;
      }

      if (has_ped != had_local_ped) {
        had_local_ped = has_ped;
      }
    }
  }

  GameFeatureLoop::GameFeatureLoop() : ScriptBase{"GameFeatureLoop"} {}

  void GameFeatureLoop::OnInit() {
    LOG_INFO("GameFeatureLoop initialized.");
  }

  void GameFeatureLoop::OnTick() {
    UpdateGameGlobals();

    if (ui::kMENU_RENDERER) {
      DisableGameUIInteractWhenMenuOpened();
    }

    if (render::kRENDERER && render::kRENDERER->IsCursorVisible()) {
      natives::PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
    }

    if (options::kOPTION_REGISTRY) {
      const auto all_opt = options::kOPTION_REGISTRY->GetAllOptions();
      for (auto&& opt : all_opt) {
        if (opt && opt->IsTickable() && opt->GetThreadType() == options::BaseOption::ThreadType::kGAME_SCRIPT) {
          opt->Tick();
        }
      }
    }
  }
}
