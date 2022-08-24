//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_GLOBALS_HPP
#define GTABASE_GLOBALS_HPP
#include <atomic>
#include <robin_hood.h>

namespace gta_base {
  namespace common {
    struct KeyState {
      bool down = false;
      std::uint64_t released_at = 0;
    };

    namespace globals {
      constexpr auto name = "gta-base";
      constexpr auto version = "1.0.0";
      constexpr auto target_window_name = "";
      constexpr auto target_window_class = "grcWindow";

      extern std::atomic<bool> running;
      extern HINSTANCE dll_handle;
      extern robin_hood::unordered_map<std::uint32_t, KeyState> key_state;
    }
  }
}
#endif //GTABASE_GLOBALS_HPP
