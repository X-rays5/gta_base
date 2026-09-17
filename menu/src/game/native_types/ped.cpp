//
// Created by X-ray on 15/09/2026.
//

#include "ped.hpp"
#include "../../natives/natives_gen9.hpp"

namespace base::menu::game {
  bool Ped::IsInAnyVehicle(const bool includeEntering) const {
    if (!IsValid()) {
      return false;
    }

    return natives::PED::IS_PED_IN_ANY_VEHICLE(*this, includeEntering);
  }

  StatusOr<Vehicle> Ped::GetVehicle(const bool includeEntering) const {
    if (!IsValid()) {
      return MakeFailure<ResultCode::kNOT_FOUND>();
    }

    return natives::PED::GET_VEHICLE_PED_IS_IN(*this, includeEntering);
  }

  bool Ped::IntoVehicle(const Vehicle& vehicle, const int seat) const {
    if (!IsValid()) {
      NOTIFY_ERR("label/teleport", "label/teleport_invalid_ped");
      return false;
    }

    if (!vehicle.Exists()) {
      NOTIFY_ERR("label/teleport", "label/teleport_invalid_vehicle");
      return false;
    }

    natives::TASK::TASK_ENTER_VEHICLE(*this, vehicle, 10000, seat, 1.0f, 16, nullptr);
    natives::VEHICLE::SET_VEHICLE_ENGINE_ON(vehicle, true, true, false);
    return true;
  }

  bool Ped::IntoVehicle(const Vehicle& vehicle) const {
    const auto seat = vehicle.GetFreeSeat();
    if (!seat) {
      NOTIFY_ERR("label/teleport", "label/teleport_no_free_seat");
      return false;
    }

    return IntoVehicle(vehicle, *seat);
  }

  bool Ped::Teleport(const rage::Vector3& position, const bool withVehicle) const {
    if (!IsValid()) {
      NOTIFY_ERR("label/teleport", "label/teleport_invalid_ped");
      return false;
    }

    LOG_DEBUG("Teleporting ped to position: x={}, y={}, z={}", position.x, position.y, position.z);

    // Gated on the in-vehicle flag and not on the handle alone: after a ped gets out, the game keeps
    // m_pMyVehicle pointing at the vehicle it left (SetPedOutOfVehicle clears it only when the ped
    // carries CPED_CONFIG_FLAG_ResetLastVehicleOnVehicleExit, which it usually does not), while
    // CPED_CONFIG_FLAG_InVehicle - the flag IS_PED_IN_ANY_VEHICLE reads and GET_VEHICLE_PED_IS_IN does
    // not - is cleared on the way out. On foot the bare handle is therefore the parked car beside the
    // ped, and a teleport meant to move the ped would move that car instead of it.
    if (withVehicle && IsInAnyVehicle(false)) {
      if (const auto vehicle = GetVehicle(false); vehicle.has_value() && vehicle->Exists()) {
        LOG_DEBUG("Ped is in a vehicle, teleporting vehicle instead. {}", vehicle->Get());
        return vehicle.value().SetCoords(position);
      }
    }

    LOG_DEBUG("Ped is not in a vehicle or withVehicle is false, teleporting ped directly.");
    return SetCoords(position);
  }
}
