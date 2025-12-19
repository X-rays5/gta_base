//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include "entity.hpp"

namespace base::rage {
  class Ped : public Entity {
  public:
    Ped(const std::int32_t handle) :
      Entity(handle) {}
  };
}