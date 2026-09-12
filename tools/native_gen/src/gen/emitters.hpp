#pragma once

#include <expected>
#include <string>

#include "../config/config.hpp"
#include "../db/model.hpp"

namespace base::tools::native_gen::gen {
  [[nodiscard]] std::string EmitNativesHeader(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
  [[nodiscard]] std::string EmitNativesImpl(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
  [[nodiscard]] std::string EmitCrossmapHeader(const config::Config& config, const db::Crossmap& crossmap);
  [[nodiscard]] std::string EmitCrossmapImpl(const config::Config& config, const db::Crossmap& crossmap);
  [[nodiscard]] std::string EmitAngelScriptHeader(const config::Config& config);

  // Unlike the other emitters this can fail: a native whose type has no ABI-safe AngelScript
  // spelling has to stop the run rather than be registered with a wrong one.
  [[nodiscard]] std::expected<std::string, std::string> EmitAngelScriptImpl(const config::Config& config,
                                                                           const db::Crossmap& crossmap,
                                                                           const db::NativeDb& db);
  [[nodiscard]] std::string EmitNativeLookup(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db);
}
