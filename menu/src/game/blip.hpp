//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "handles.hpp"
#include <rage/vector.hpp>

namespace base::menu::game {
  /// A blip on the map. Not an entity: it has a place on the map, not one in the world, which is why it
  /// hangs off the handle root rather than off Entity.
  class Blip : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;

    /// The sprite the game gives the player's own waypoint. BlipIcons is not ported, so the one number
    /// this class needs is spelled out here instead of pulled from the enum.
    static constexpr int kWAYPOINT_SPRITE = 8;

    /// The first blip carrying `sprite`, and `colour` when one is given (-1 takes any). Blips are looked
    /// up by icon - there is no map from anything else to a blip - so this walks the game's own list.
    /// A null handle when none of them match.
    static Blip FindFirst(int sprite, int colour = -1);
    /// Wherever the player last dropped their waypoint.
    static Blip Waypoint();

    [[nodiscard]] bool Exists() const;
    [[nodiscard]] rage::Vector3 GetCoords() const;
    [[nodiscard]] int GetColour() const;
  };
  static_assert(sizeof(Blip) == sizeof(ScrHandle));
}
