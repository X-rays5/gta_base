//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "entity.hpp"
#include "vehicle.hpp"

namespace base::menu::game {
  /// A ped, the local player's included.
  class Ped : public Entity {
  public:
    using Entity::Entity;

    [[nodiscard]] bool IsInAnyVehicle(bool includeEntering = false) const;
    /// The vehicle this ped is in. `includeEntering` also counts one it is on its way into, which is
    /// what makes a handle usable as soon as the player opens the door.
    [[nodiscard]] StatusOr<Vehicle> GetVehicle(bool includeEntering = false) const;

    /// Puts the ped in a given seat. Whether the seat is free is not asked here: a caller that cares
    /// picks one with Vehicle::GetFreeSeat() first.
    [[nodiscard]] bool IntoVehicle(const Vehicle& vehicle, int seat) const;
    /// The same, picking a free seat - false when the vehicle has none.
    [[nodiscard]] bool IntoVehicle(const Vehicle& vehicle) const;

    /// Moves the ped, riding along in its vehicle when that is wanted and it is in one. Without a
    /// vehicle to bring there is nothing that can fail, so the caller has nothing to check here: what a
    /// teleport could not do is visible in where the ped then is.
    [[nodiscard]] bool Teleport(const rage::Vector3& position, bool withVehicle = false) const;
  };
  static_assert(sizeof(Ped) == sizeof(ScrHandle));
}
