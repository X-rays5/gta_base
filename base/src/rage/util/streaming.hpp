//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_STREAMING_HPP
#define GTA_BASE_STREAMING_HPP
#include "../classes/scr_vector.hpp"

namespace rage::util {
  /// @note This should be called in a fiber job
  scrVector LoadGroundAtCoord(scrVector coords);
}
#endif //GTA_BASE_STREAMING_HPP
