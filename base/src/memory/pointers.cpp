//
// Created by X-ray on 3/7/2022.
//

#include "pointers.hpp"

namespace gta_base {
  namespace memory {
    Pointers::Pointers() {
      game_state_ = Signature("83 3D ? ? ? ? ? 75 17 8B 43 20").Scan("game_state").Add(2).Rip().As<rage::GameState*>();

      ped_factory_ = Signature("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81").Scan("ped_factory").Add(3).Rip().As<CPedFactory**>();
    }
  }
}