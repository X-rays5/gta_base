//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include <atomic>
#include "native_types.hpp"

namespace base::menu::game::globals {
  struct LocalPlayer {
    std::atomic<natives::Player> player_id{0};
    std::atomic<natives::Ped> ped_id{0};

    std::atomic<natives::Vehicle> vehicle_id{0};
    std::atomic<natives::Vehicle> vehicle_include_entering{0};
    std::atomic<natives::Vehicle> last_vehicle{0};

    bool IsValidPedId() const {
      return ped_id.load().IsValid();
    }

    bool IsInVehicle() const {
      return vehicle_id.load().IsValid();
    }
  };

  extern LocalPlayer local_player;
}
