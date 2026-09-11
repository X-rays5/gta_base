#pragma once

#include <string_view>

namespace base::tools::native_gen {
  // Output file names. These are fixed rather than configurable because the generated
  // sources include each other by these exact names.
  inline constexpr std::string_view kOutputHeader = "natives_gen9.hpp";
  inline constexpr std::string_view kOutputImpl = "natives_gen9.cpp";
  inline constexpr std::string_view kOutputCrossmapHeader = "crossmap.hpp";
  inline constexpr std::string_view kOutputCrossmapImpl = "crossmap.cpp";
  inline constexpr std::string_view kOutputSol2Header = "natives_sol2.hpp";
  inline constexpr std::string_view kOutputSol2Impl = "natives_sol2.cpp";
  inline constexpr std::string_view kOutputAsHeader = "natives_as.hpp";
  inline constexpr std::string_view kOutputAsImpl = "natives_as.cpp";
  inline constexpr std::string_view kOutputLookup = "native_lookup.hpp";

  inline constexpr std::string_view kDefaultUrl =
      "https://raw.githubusercontent.com/alloc8or/gta5-nativedb-data/master/natives_gen9.json";
}
