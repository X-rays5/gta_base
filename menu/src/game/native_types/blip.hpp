//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include <rage/vector.hpp>
#include "handles.hpp"
#include "../enums/blip_color.hpp"
#include "../enums/blip_icon.hpp"

namespace base::menu::game {
  /// A blip on the map. Not an entity: it has a place on the map, not one in the world, which is why it
  /// hangs off the handle root rather than off Entity.
  class Blip : public ScrHandle {
  public:
    /// The first blip carrying `Icon`, and `Colour` when one is given (-1 takes any). Blips are looked
    /// up by icon - there is no map from anything else to a blip - so this walks the game's own list.
    static std::optional<Blip> FindFirst(int sprite, int colour = -1);

    static std::optional<Blip> FindFirst(BlipIcon sprite, BlipColor colour = BlipColor::kANY) {
      return FindFirst(static_cast<int>(sprite), static_cast<int>(colour));
    }

    /// Wherever the player last dropped their waypoint.
    static std::optional<Blip> Waypoint();

    static std::optional<Blip> Objective();

    [[nodiscard]] bool Exists() const;
    [[nodiscard]] rage::Vector3 GetCoords() const;
    [[nodiscard]] int GetColour() const;
  };

  static_assert(sizeof(Blip) == sizeof(ScrHandle));
}
