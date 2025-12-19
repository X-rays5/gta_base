//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include <rage/vector.hpp>
#include "../rage/entity/entity.hpp"
#include "../rage/entity/ped.hpp"
#include "../rage/entity/player.hpp"
#include "../rage/entity/vehicle.hpp"

namespace base::menu::natives {
  using Any = std::uint64_t;
  using Object = std::int32_t;
  using FireId = std::int32_t;
  using Blip = std::int32_t;
  using Interior = std::int32_t;
  using Pickup = std::int32_t;
  using Hash = std::uint32_t;
  using Cam = std::int32_t;
  using Entity = rage::Entity;
  using ScrHandle = Entity;
  using Ped = rage::Ped;
  using Player = rage::Player;
  using Vehicle = rage::Vehicle;
  using Vector3 = ::rage::Vector3;
}