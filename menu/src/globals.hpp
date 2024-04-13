//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_GLOBALS_852FF53BDBCA440E9F3C9A229E4D34B1_HPP
#define GTA_BASE_GLOBALS_852FF53BDBCA440E9F3C9A229E4D34B1_HPP
#include <atomic>

namespace base::globals {
  constexpr static auto kBASE_NAME = "base";
  extern std::atomic<bool> kRUNNING;
  constexpr static auto target_window_name = "";
  constexpr static auto target_window_class = "grcWindow";
  constexpr static auto target_process_name = "GTA5.exe";
}
#endif //GTA_BASE_GLOBALS_852FF53BDBCA440E9F3C9A229E4D34B1_HPP
