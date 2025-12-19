//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include "entity.hpp"

namespace base::rage {
  class Vehicle : public Entity {
  public:
    Vehicle(const std::int32_t handle) :
      Entity(handle) {}
  };
}