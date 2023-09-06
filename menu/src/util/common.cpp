//
// Created by xray on 06/09/2023.
//

#include "common.hpp"

namespace base::util::common {
  std::uint64_t GetTimeStamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }

  std::string AddrToHex(uint64_t addr) {
    if (!addr)
      return "0x0";

    std::ostringstream stream;
    stream << "0x" << std::hex << std::uppercase << addr;
    return stream.str();
  }
}