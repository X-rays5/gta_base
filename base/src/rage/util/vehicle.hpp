//
// Created by X-ray on 11/13/22.
//

#pragma once
#ifndef GTA_BASE_UTIL_VEHICLE_HPP
#define GTA_BASE_UTIL_VEHICLE_HPP
#include "../types.hpp"

namespace rage::util::vehicle {
void SetMpBitset(Vehicle veh);
Vector3 GetSpawnLocation(bool spawn_inside);
Vehicle Spawn(Hash hash, Vector3 pos, float heading, bool is_networked = true);
}
#endif //GTA_BASE_UTIL_VEHICLE_HPP
