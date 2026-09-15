//
// Created by X-ray on 15/09/2026.
//

#include "blip.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::game {
  Blip Blip::FindFirst(const int sprite, const int colour) {
    for (auto blip = natives::HUD::GET_FIRST_BLIP_INFO_ID(sprite); blip.Exists();
         blip = natives::HUD::GET_NEXT_BLIP_INFO_ID(sprite)) {
      if (colour == -1 || blip.GetColour() == colour) {
        return blip;
      }
    }
    return {};
  }

  Blip Blip::Waypoint() {
    return FindFirst(kWAYPOINT_SPRITE);
  }

  bool Blip::Exists() const {
    return natives::HUD::DOES_BLIP_EXIST(*this);
  }

  rage::Vector3 Blip::GetCoords() const {
    return natives::HUD::GET_BLIP_COORDS(*this);
  }

  int Blip::GetColour() const {
    return natives::HUD::GET_BLIP_COLOUR(*this);
  }
}
