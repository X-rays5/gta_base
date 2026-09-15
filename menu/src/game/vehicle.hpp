//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "entity.hpp"
#include <optional>

namespace base::menu::game {
  /// A vehicle.
  class Vehicle : public Entity {
  public:
    using Entity::Entity;

    /// Every seat the game can put a new handle in, front left first: -1 is the driver's, -2 the one
    /// beside it. No seat at all is an empty optional. `isTaskRunning` is the game's own question of
    /// whether a ped already walking to a seat counts as it being taken.
    static constexpr int kDRIVER_SEAT = -1;
    static constexpr int kPASSENGER_SEAT = -2;

    /// Body and deformation, so a repaired vehicle looks repaired rather than just being undamaged.
    void Fix() const;

    [[nodiscard]] bool IsSeatFree(int seat, bool isTaskRunning = true) const;
    [[nodiscard]] std::optional<int> GetFreeSeat(bool isTaskRunning = true) const;

    /// Moves the vehicle and drops it back onto the ground properly, so it arrives standing rather than
    /// halfway into it. Control is asked for first when the vehicle is not ours to move - whether the
    /// answer arrives in time to matter is the caller's business, see RequestControl().
    void Teleport(const rage::Vector3& position) const;
  };
  static_assert(sizeof(Vehicle) == sizeof(ScrHandle));
}
