//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include <fmt/format.h>
#include <rage/vector.hpp>

namespace base::menu::natives {
  using Any = std::uint32_t;
  using Object = std::int32_t;
  using FireId = std::int32_t;
  using Blip = std::int32_t;
  using Interior = std::int32_t;
  using Pickup = std::int32_t;
  using Hash = std::uint32_t;
  using Cam = std::int32_t;
  using Entity = std::int32_t;
  using ScrHandle = Entity;
  using Ped = Entity;
  using Player = Entity;
  using Vehicle = Entity;
  using Vector3 = ::rage::Vector3;
}

template <>
struct fmt::formatter<::rage::Vector3> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const ::rage::Vector3& v3, FormatContext& ctx) const {
    return formatter<std::string_view>::format(fmt::format("(x={},y={},z={})", v3.x, v3.y, v3.z), ctx);
  }
};