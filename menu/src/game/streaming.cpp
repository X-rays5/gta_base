//
// Created by X-ray on 17/09/2026.
//

#include "streaming.hpp"

#include "../natives/natives_gen9.hpp"

namespace base::menu::game::streaming {
  std::float_t GetZCoordsAtPoint(const rage::Vector3& coords) {
    constexpr int max_attempts = 20;
    float ground_z = coords.z;
    int current_attempts = 0;
    bool found_ground = false;

    do {
      natives::STREAMING::REQUEST_COLLISION_AT_COORD(coords.x, coords.y, coords.z);

      // find ground first and if found set z to new value and break
      if (constexpr float max_ground_check = 1000.f; natives::MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, max_ground_check, &ground_z, false, false)) {
        ground_z = ground_z + 1.0f;
        found_ground = true;
        break;
      }

      // if (current_attempts % 3 == 0) {
      //   ground_z += 10.f;
      // }

      ++current_attempts;
      minicoropp::this_coro::yield();
    } while (current_attempts < max_attempts);

    // whether ground was found or not check for water height
    float water_height;
    if (natives::WATER::GET_WATER_HEIGHT(coords.x, coords.y, coords.z, &water_height)) {
      return water_height;
    }

    // fallback value
    if (!found_ground)
      return natives::PATH::GET_APPROX_HEIGHT_FOR_POINT(coords.x, coords.y);

    return ground_z;
  }
}
