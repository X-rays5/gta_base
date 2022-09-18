//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_TELEPORT_HPP
#define GTA_BASE_TELEPORT_HPP
#include <rage/vector.hpp>
#include "../types.hpp"

namespace rage {
  namespace util {
    /// @note This should be called in a fiber job
    bool Teleport(rage::fvector3 coords, bool with_vehicle);
    /// @note This should be called in a fiber job
    bool TeleportIntoVehicle(Vehicle veh_id);
    /// @note This should be called in a fiber job
    bool TeleportToBlip(int sprite, bool with_vehicle);
    /// @note This should be called in a fiber job
    bool TeleportToWayPoint(bool with_vehicle);
    /// @note This should be called in a fiber job
    bool TeleportToObjective(bool with_vehicle);
  }
}
#endif //GTA_BASE_TELEPORT_HPP
