//
// Created by X-ray on 24/03/2026.
//

#include "feature_loop.hpp"
#include "../natives/natives_gen9.hpp"
#include "../ui/menu_renderer.hpp"
#include "feature_settings.hpp"

namespace base::menu::feature {
  namespace {
    thread_local natives::Player player_id;
    thread_local natives::Ped player_ped;
    thread_local bool is_player_in_vehicle;
    thread_local natives::Vehicle player_vehicle;

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

    void SelfHealth() {
      const auto settings = kSETTINGS->getSettings().lock();
      if (settings->self.health.god_mode) {
        natives::ENTITY::SET_ENTITY_INVINCIBLE(player_ped, true, false);
      } else if (settings->self.health.semi_god_mode) {
        natives::ENTITY::SET_ENTITY_HEALTH(player_ped, natives::ENTITY::GET_ENTITY_MAX_HEALTH(player_ped), player_ped, NULL);
      } else {
        natives::ENTITY::SET_ENTITY_INVINCIBLE(player_ped, false, false);
      }
    }

    void SelfOptions() {
      SelfHealth();

      const auto settings = kSETTINGS->getSettings().lock();
      if (settings->self.force_wanted_level) {
        natives::PLAYER::SET_PLAYER_WANTED_LEVEL(player_id, settings->self.wanted_level_force.load(), false);
        natives::PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player_id, false);
      } else if (settings->self.never_wanted) {
        natives::PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player_id);
        natives::PLAYER::SUPPRESS_WITNESSES_CALLING_POLICE_THIS_FRAME(player_id);
      }
    }
  }

  GameFeatureLoop::GameFeatureLoop() : ScriptBase{"GameFeatureLoop"} {}

  void GameFeatureLoop::OnInit() {
    LOG_INFO("GameFeatureLoop initialized.");
  }

  void GameFeatureLoop::OnTick() {
    player_id = natives::PLAYER::PLAYER_ID();
    player_ped = natives::PLAYER::PLAYER_PED_ID();
    is_player_in_vehicle = natives::PED::IS_PED_IN_ANY_VEHICLE(player_ped, false);
    if (is_player_in_vehicle) {
      player_vehicle = natives::PED::GET_VEHICLE_PED_IS_IN(player_ped, false);
    } else {
      player_vehicle = 0;
    }

    if (ui::kMENU_RENDERER) {
      DisableGameUIInteractWhenMenuOpened();
    }

    if (kSETTINGS == nullptr)
      return;

    SelfOptions();
  }
}
