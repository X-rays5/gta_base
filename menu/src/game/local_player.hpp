//
// Created by X-ray on 16/09/2026.
//

#pragma once
#include "globals.hpp"
#include "native_types.hpp"

namespace base::menu::game {
  class LocalPlayer : public Player {
  public:
    static LocalPlayer Get() {
      return LocalPlayer(globals::local_player.player_id);
    }

    bool TeleportToWaypoint() const;
    bool TeleportToObjective() const;

    StatusOr<Vehicle> GetCurrentVehicle() const;
    StatusOr<Vehicle> GetLastVehicle() const;

  private:
    explicit constexpr LocalPlayer(const std::int32_t id) : Player(id) {}
  };}