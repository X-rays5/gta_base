//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_GLOBALS_HPP
#define GTABASE_GLOBALS_HPP
#include <atomic>

namespace gta_base {
  namespace common {
    namespace globals {
      constexpr auto name = "gta-base";
      constexpr auto version = "1.0.0";
      constexpr auto target_window_name = "";
      constexpr auto target_window_class = "Notepad";

      extern std::atomic<bool> running;
    }
  }
}
#endif //GTABASE_GLOBALS_HPP
