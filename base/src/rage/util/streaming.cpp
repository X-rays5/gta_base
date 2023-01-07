//
// Created by X-ray on 09/18/22.
//

#include "streaming.hpp"
#include "../../fiber/pool.hpp"
#include "../../fiber/script.hpp"

namespace rage::util {
scrVector LoadGroundAtCoord(scrVector coords) {
  float ground_z;
  scrVector result = coords;

  for (int i = 0; i < 10; i++) { // try 10 times max
    // Only request a collision after the first try failed because the location might already be loaded on first attempt.
    for (int z = 0; z < 1000; z += 100) {
      STREAMING::REQUEST_COLLISION_AT_COORD(result.x, result.y, (float) z);

      gta_base::fiber::Script::GetCurr()->Yield();
    }

    if (MISC::GET_GROUND_Z_FOR_3D_COORD(result.x, result.y, 1000.f, &ground_z, false, false)) {
      result.z = ground_z + 1.f;

      return result;
    }
  }

  result.z = 1000.f;

  return result;
}
}