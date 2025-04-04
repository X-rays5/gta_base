//
// Created by X-ray on 11/02/2025.
//

#include "time.hpp"
#include <chrono>

namespace base::common::util::time {
  std::uint64_t GetTimeStamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }
}