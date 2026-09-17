//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include <atomic>
#include "native_types.hpp"

namespace base::menu::game::globals {
  struct LocalPlayer {
    natives::Player player_id{0};
    natives::Ped ped_id{0};

    natives::Vehicle vehicle{0};
    natives::Vehicle vehicle_include_entering{0};
    natives::Vehicle last_vehicle{0};

    bool IsValidPedId() const {
      return ped_id.IsValid();
    }

    bool IsInVehicle() const {
      return vehicle.IsValid();
    }
  };

  extern LocalPlayer local_player;
}
