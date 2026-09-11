#pragma once

#include <expected>
#include <string>

#include "../config/config.hpp"

namespace base::tools::native_gen::db {
  struct NativeDbDocument {
    std::string body;
    bool from_cache{false};
  };

  // Returns the raw native database JSON, preferring the local cache. The body is never
  // re-serialised: key order in the document is significant.
  [[nodiscard]] std::expected<NativeDbDocument, std::string> LoadNativeDbDocument(const config::Settings& settings,
                                                                                  bool refresh);
}
