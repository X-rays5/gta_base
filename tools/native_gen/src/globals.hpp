#pragma once

#include <memory>

#include <base-common/logging/logger.hpp>

namespace base::tools::native_gen::globals {
  // Outlives every log call: the logger is created in main and torn down at exit, which is
  // what flushes the log file.
  inline std::unique_ptr<common::logging::Manager> kLOGGER;

  // Verbose lines are emitted at info level and gated on this instead of using LOG_DEBUG,
  // which logging_macro.hpp compiles out whenever NDEBUG is set.
  inline bool kVERBOSE = false;
}
