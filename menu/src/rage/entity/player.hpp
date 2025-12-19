//
// Created by X-ray on 19/12/2025.
//

#pragma once

#include "ped.hpp"

namespace base::rage {
  class Player : public Ped {
  public:
    Player(const std::int32_t handle) :
      Ped(handle) {}
  };
}