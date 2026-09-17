//
// Created by X-ray on 15/09/2026.
//

#include "blip.hpp"
#include "../../natives/natives_gen9.hpp"

namespace base::menu::game {
  std::optional<Blip> Blip::FindFirst(const int sprite, const int colour) {
    for (auto blip = natives::HUD::GET_FIRST_BLIP_INFO_ID(sprite); blip.Exists(); blip = natives::HUD::GET_NEXT_BLIP_INFO_ID(sprite)) {
      if (colour == -1 || blip.GetColour() == colour) {
        return blip;
      }
    }
    return std::nullopt;
  }

  std::optional<Blip> Blip::Waypoint() {
    return FindFirst(BlipIcon::kRADAR_WAYPOINT);
  }

  std::optional<Blip> Blip::Objective() {
    std::optional<Blip> res;

    if ((res = FindFirst(BlipIcon::kRADAR_LEVEL, BlipColor::kYELLOW)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_LEVEL, BlipColor::kYELLOW_ORANGE)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_LEVEL, BlipColor::kBLUE_FRIENDLY_4)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_RACEFLAG, BlipColor::kWHITE)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_LEVEL, BlipColor::kGREEN)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_LEVEL, BlipColor::kBLUE)))
      return res;
    if ((res = FindFirst(BlipIcon::kRADAR_CRATEDROP)))
      return res;

    static const int blips[] = {1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288};
    for (const auto& blip: blips) {
      if ((res = FindFirst(blip, 5)))
        return res;
    }

    return std::nullopt;
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
