//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_BLIP_HPP
#define GTA_BASE_BLIP_HPP
#include <rage/vector.hpp>
#include "../types.hpp"

namespace rage {
  namespace util {
    /// @note This should be called on the GTAV script thread
    rage::scrVector GetBlipLocation(int sprite, int color = -1);
    /// @note This should be called on the GTAV script thread
    rage::scrVector GetWaypointLocation();
    /// @note This should be called on the GTAV script thread
    rage::scrVector  GetObjectiveLocation();
  }
}
#endif //GTA_BASE_BLIP_HPP
