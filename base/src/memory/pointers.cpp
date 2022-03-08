//
// Created by X-ray on 3/7/2022.
//

#include "pointers.hpp"

namespace gta_base {
  namespace memory {
    Pointers::Pointers() {
      game_state_ = Signature("83 3D ? ? ? ? ? 75 17 8B 43 20").Scan("game_state").Add(2).Rip().As<decltype(game_state_)>();
    }
  }
}