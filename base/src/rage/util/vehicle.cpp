//
// Created by X-ray on 11/13/22.
//

#include "vehicle.hpp"
#include "../../fiber/script.hpp"
#include "../../memory/pointers.hpp"

namespace rage::util::vehicle {
  void SetMpBitset(Vehicle veh) {
    DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
    auto networkId = NETWORK::VEH_TO_NET(veh);
    if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
      NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
    VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);
  }

  Vector3 GetSpawnLocation(bool spawn_inside) {
    float y_offset = 0;

    if (gta_base::globals::local_player.vehicle_id != 0) {
      y_offset = 10.f;
    } else if (!spawn_inside) {
      y_offset = 5.f;
    }

    return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(gta_base::globals::local_player.ped_id, 0.f, y_offset, 0.f);
  }

  Vehicle Spawn(Hash hash, Vector3 pos, float heading, bool is_networked) {
    LOG_INFO("Spawning vehicle with hash: {:x}", hash);
    for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++) {
      STREAMING::REQUEST_MODEL(hash);
      gta_base::fiber::Script::GetCurr()->Yield();
    }

    if (!STREAMING::HAS_MODEL_LOADED(hash)) {
      return 0;
    }

    *(unsigned short*) gta_base::memory::kPOINTERS->model_spawn_bypass_ = 0x9090;
    auto veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, heading, is_networked, false, false);
    *(unsigned short*) gta_base::memory::kPOINTERS->model_spawn_bypass_ = 0x0574;

    gta_base::fiber::Script::GetCurr()->Yield();

    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

    if (gta_base::common::IsSessionStarted()) {
      SetMpBitset(veh);
    }

    return veh;
  }
}