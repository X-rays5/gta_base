//
// Created by X-ray on 3/7/2022.
//

#include "pointers.hpp"

namespace gta_base {
  namespace memory {
    Pointers::Pointers() {
      game_state_ = Signature("48 85 C9 74 4B 83 3D").Scan("game_state").Add(7).Rip().As<decltype(game_state_)>();
    }
  }
}