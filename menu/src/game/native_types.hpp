//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <rage/vector.hpp>
#include "blip.hpp"
#include "entity.hpp"
#include "handles.hpp"
#include "ped.hpp"
#include "player.hpp"
#include "vehicle.hpp"

namespace base::menu::natives {
  using Any = std::uint32_t;
  using Hash = std::uint32_t;

  using Object = game::Object;
  using FireId = game::FireId;
  using Blip = game::Blip;
  using Interior = game::Interior;
  using Pickup = game::Pickup;
  using Cam = game::Cam;
  using Entity = game::Entity;
  using ScrHandle = game::ScrHandle;
  using Ped = game::Ped;
  using Player = game::Player;
  using Vehicle = game::Vehicle;
  using Vector3 = ::rage::Vector3;
}

template <>
struct fmt::formatter<::rage::Vector3> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const ::rage::Vector3& v3, FormatContext& ctx) const {
    return formatter<std::string_view>::format(fmt::format("(x={},y={},z={})", v3.x, v3.y, v3.z), ctx);
  }
};
