//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_POINTERS_HPP
#define GTABASE_POINTERS_HPP
#include <memory>
#include <d3d11.h>
#include "sigscanner.hpp"
#include "../rage/enums.hpp"
#include "../rage/classes.hpp"

namespace gta_base {
  namespace memory {
    class Pointers {
    public:
      Pointers();
      ~Pointers();

      using send_minidump_t = char(*)();

      enum rage::GameState* game_state_{};
      CPedFactory** ped_factory_{};
      IDXGISwapChain** swap_chain_{};
      send_minidump_t send_minidump{};
    };
    inline Pointers* kPOINTERS{};
  }
}
#endif //GTABASE_POINTERS_HPP
