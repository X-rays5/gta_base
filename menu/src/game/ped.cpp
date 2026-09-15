//
// Created by X-ray on 15/09/2026.
//

#include "ped.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::game {
  bool Ped::IsInAnyVehicle(const bool includeEntering) const {
    return natives::PED::IS_PED_IN_ANY_VEHICLE(*this, includeEntering);
  }

  Vehicle Ped::GetVehicle(const bool includeEntering) const {
    return natives::PED::GET_VEHICLE_PED_IS_IN(*this, includeEntering);
  }

  void Ped::IntoVehicle(const Vehicle& vehicle, const int seat) const {
    natives::PED::SET_PED_INTO_VEHICLE(*this, vehicle, seat);
  }

  bool Ped::IntoVehicle(const Vehicle& vehicle) const {
    const auto seat = vehicle.GetFreeSeat();
    if (!seat) {
      return false;
    }

    IntoVehicle(vehicle, *seat);
    return true;
  }

  void Ped::Teleport(const rage::Vector3& position, const bool withVehicle) const {
    if (!IsValid()) {
      return;
    }

    if (withVehicle) {
      const auto vehicle = GetVehicle(false);
      if (vehicle.IsValid()) {
        vehicle.Teleport(position);
        return;
      }
    }

    SetCoords(position);
  }
}
