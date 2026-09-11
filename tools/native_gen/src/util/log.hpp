#pragma once

#include <base-common/logging/logging_macro.hpp>

#include "../globals.hpp"

// Verbose lines go out at info level and are suppressed at runtime rather than being
// compiled out, because logging_macro.hpp pins SPDLOG_ACTIVE_LEVEL to INFO under NDEBUG
// and this tool is documented to be built in Release.
#define LOG_DETAIL(...)                                  \
  do {                                                   \
    if (base::tools::native_gen::globals::kVERBOSE) {     \
      LOG_INFO(__VA_ARGS__);                             \
    }                                                    \
  } while (0)

namespace base::tools::native_gen::util {
  // Manager(false) leaves the logger with its file sink only. This adds the console sink the
  // tool prints through and applies the run's verbosity.
  void SetUpConsoleLogging(bool quiet);
}
