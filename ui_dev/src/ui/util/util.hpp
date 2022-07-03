//
// Created by X-ray on 07/03/22.
//

#pragma once
#ifndef GTA_BASE_UTIL_HPP
#define GTA_BASE_UTIL_HPP
#include <chrono>
#include <cstdint>

namespace ui {
  namespace util {
    inline std::uint64_t UnixEpoch() {
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
  }
}
#endif //GTA_BASE_UTIL_HPP
