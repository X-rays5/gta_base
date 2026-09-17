//
// Created by X-ray on 15/09/2026.
//

#include "vehicle.hpp"
#include "../../natives/natives_gen9.hpp"

namespace base::menu::game {
  void Vehicle::Fix() const {
    natives::VEHICLE::SET_VEHICLE_FIXED(*this);
    natives::VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(*this);
  }

  bool Vehicle::IsSeatFree(const int seat, const bool isTaskRunning) const {
    if (!IsValid()) {
      LOG_ERROR("Vehicle::IsSeatFree called on invalid vehicle handle");
      return false;
    }

    return natives::VEHICLE::IS_VEHICLE_SEAT_FREE(*this, seat, isTaskRunning);
  }

  std::optional<int> Vehicle::GetFreeSeat(const bool isTaskRunning) const {
    if (IsSeatFree(kDRIVER_SEAT, isTaskRunning)) {
      return kDRIVER_SEAT;
    }

    if (IsSeatFree(kPASSENGER_SEAT, isTaskRunning)) {
      return kPASSENGER_SEAT;
    }
    return std::nullopt;
  }
}
