//
// Created by X-ray on 16/09/2026.
//

#include "local_player.hpp"
#include "globals.hpp"
#include "native_types.hpp"
#include "streaming.hpp"

namespace base::menu::game {
  bool LocalPlayer::TeleportToWaypoint() const {
    const auto waypoint = Blip::Waypoint();
    if (!waypoint) {
      NOTIFY_ERR("label/teleport", "label/teleport/no_waypoint");
      return false;
    }

    auto coords = waypoint->GetCoords();
    coords.z = streaming::GetZCoordsAtPoint(coords);
    if (const auto ped = GetPed(); ped.Exists()) {
      if (ped.IsInAnyVehicle(true)) {
        if (auto vehicle = ped.GetVehicle(true); vehicle.has_value() && vehicle->Exists()) {
          return vehicle.value().SetCoords(coords, true);
        }
      }

      return ped.SetCoords(coords, false);
    }

    NOTIFY_ERR("label/teleport", "label/teleport/failed");
    return false;
  }

  bool LocalPlayer::TeleportToObjective() const {
    const auto objective = Blip::Objective();
    if (!objective) {
      NOTIFY_ERR("label/teleport", "label/teleport/no_objective");
      return false;
    }

    auto coords = objective->GetCoords();
    coords.z = streaming::GetZCoordsAtPoint(coords);
    if (const auto ped = GetPed(); ped.Exists()) {
      if (ped.IsInAnyVehicle(true)) {
        if (auto vehicle = ped.GetVehicle(true); vehicle.has_value() && vehicle->Exists()) {
          return vehicle.value().SetCoords(coords, true);
        }
      }

      return ped.SetCoords(coords, false);
    }

    NOTIFY_ERR("label/teleport", "label/teleport/failed");
    return false;
  }

  StatusOr<Vehicle> LocalPlayer::GetCurrentVehicle() const {
    if (const auto ped = GetPed(); ped.Exists()) {
      if (globals::local_player.vehicle != 0) {
        return globals::local_player.vehicle;
      }
    }

    return MakeFailure<ResultCode::kNOT_FOUND>();
  }

  StatusOr<Vehicle> LocalPlayer::GetLastVehicle() const {
    if (const auto ped = GetPed(); ped.Exists()) {
      if (globals::local_player.last_vehicle != 0) {
        return globals::local_player.last_vehicle;
      }
    }

    return MakeFailure<ResultCode::kNOT_FOUND>();
  }
}
