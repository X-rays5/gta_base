//
// Created by X-ray on 09/18/22.
//

#include "blip.hpp"
#include "../enums.hpp"

namespace rage::util {
    rage::scrVector GetBlipLocation(int sprite, int color) {
      Blip blip;
      for (blip = HUD::GET_FIRST_BLIP_INFO_ID(sprite); HUD::DOES_BLIP_EXIST(blip) && color != -1 && HUD::GET_BLIP_COLOUR(blip) != color; blip = HUD::GET_NEXT_BLIP_INFO_ID(sprite)) {

      }

      if (!HUD::DOES_BLIP_EXIST(blip) || (color != -1 && HUD::GET_BLIP_COLOUR(blip) != color)) return {};

      return HUD::GET_BLIP_COORDS(blip);
    }

    rage::scrVector GetWaypointLocation() {
      return GetBlipLocation((int)BlipIcons::Waypoint, -1);
    }

    rage::scrVector GetObjectiveLocation() {
      rage::scrVector res;

      if ((res = GetBlipLocation((int)BlipIcons::Circle, (int)BlipColors::YellowMission))) return res;
      if ((res = GetBlipLocation((int)BlipIcons::Circle, (int)BlipColors::YellowMission2))) return res;
      if ((res = GetBlipLocation((int)BlipIcons::Circle, (int)BlipColors::Mission))) return res;
      if ((res = GetBlipLocation((int)BlipIcons::RaceFinish, (int)BlipColors::None))) return res;
      if ((res = GetBlipLocation( (int)BlipIcons::Circle, (int)BlipColors::Green))) return res;
      if ((res = GetBlipLocation( (int)BlipIcons::Circle, (int)BlipColors::Blue))) return res;
      if ((res = GetBlipLocation((int)BlipIcons::CrateDrop))) return res;

      static const int blips[] = { 1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288 };
      for (const auto& blip : blips)
      {
        if ((res = GetBlipLocation(blip, 5))) return res;
      }

      return {};
    }
  }