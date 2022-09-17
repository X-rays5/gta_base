//
// Created by X-ray on 09/17/22.
//

#include "self_globals.hpp"
#include "../misc/globals.hpp"
#include "../natives/natives.hpp"
#include "../logger/logger.hpp"

namespace gta_base {
  common::LocalPlayer globals::local_player;
  namespace looped_game {
    void SetSelfGlobals() {
      using namespace globals;

      local_player.id = PLAYER::PLAYER_ID();
      local_player.ped_id = PLAYER::PLAYER_PED_ID();
      if (local_player.alive && PLAYER::IS_PLAYER_DEAD(local_player.id)) {
        local_player.alive = false;
        local_player.SendEvent(common::LocalPlayerEvent::kDied);
      } else if (!local_player.alive && !PLAYER::IS_PLAYER_DEAD(local_player.id)) {
        local_player.alive = true;
        local_player.SendEvent(common::LocalPlayerEvent::kRespawned);
      } else {
        local_player.alive = !PLAYER::IS_PLAYER_DEAD(local_player.id);
      }
      local_player.player_pos = ENTITY::GET_ENTITY_COORDS(local_player.ped_id, false);

      if (local_player.vehicle_id == 0 && PED::IS_PED_IN_ANY_VEHICLE(local_player.ped_id, false)) {
        [[unlikely]]
        local_player.vehicle_id = PED::GET_VEHICLE_PED_IS_IN(local_player.ped_id, false);
        local_player.vehicle_pos = ENTITY::GET_ENTITY_COORDS(local_player.vehicle_id, false);
        local_player.SendEvent(common::LocalPlayerEvent::kEnterVehicle);
      } else if (local_player.vehicle_id != 0 && !PED::IS_PED_IN_ANY_VEHICLE(local_player.ped_id, false)) {
        [[unlikely]]
        local_player.vehicle_id = 0;
        local_player.vehicle_pos = 0;
        local_player.SendEvent(common::LocalPlayerEvent::kLeaveVehicle);
      } else if (PED::IS_PED_IN_ANY_VEHICLE(local_player.ped_id, false)) {
        [[likely]]
        local_player.vehicle_id = PED::GET_VEHICLE_PED_IS_IN(local_player.ped_id, false);
        local_player.vehicle_pos = ENTITY::GET_ENTITY_COORDS(local_player.vehicle_id, false);
      } else {
        [[likely]]
        local_player.vehicle_id = 0;
        local_player.vehicle_pos = 0;
      }
    }
  }
}