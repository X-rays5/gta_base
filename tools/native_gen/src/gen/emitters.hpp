#pragma once

#include <string>

#include "../config/config.hpp"
#include "../db/model.hpp"

namespace base::tools::native_gen::gen {
  [[nodiscard]] std::string EmitNativesHeader(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
  [[nodiscard]] std::string EmitNativesImpl(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
  [[nodiscard]] std::string EmitCrossmapHeader(const config::Config& config, const db::Crossmap& crossmap);
  [[nodiscard]] std::string EmitCrossmapImpl(const config::Config& config, const db::Crossmap& crossmap);
  [[nodiscard]] std::string EmitSol2Header(const config::Config& config);
  [[nodiscard]] std::string EmitSol2Impl(const config::Config& config, const db::NativeDb& db);
  [[nodiscard]] std::string EmitNativeLookup(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
}
