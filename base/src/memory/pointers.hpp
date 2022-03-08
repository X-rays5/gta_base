//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_POINTERS_HPP
#define GTABASE_POINTERS_HPP
#include <memory>
#include "sigscanner.hpp"
#include "../rage/enums.hpp"

namespace gta_base {
  namespace memory {
    class Pointers {
    public:
      Pointers();

      enum rage::GameState* game_state_{};
    };
    inline std::unique_ptr<Pointers> kPOINTERS;
  }
}
#endif //GTABASE_POINTERS_HPP
