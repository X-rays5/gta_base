#include "fetch.hpp"

#include <chrono>

#include <cpr/cpr.h>

#include "../util/log.hpp"
#include "../util/util.hpp"

namespace base::tools::native_gen::db {
  namespace {
    constexpr int kMillisecondsPerSecond = 1000;
  }

  std::expected<NativeDbDocument, std::string> LoadNativeDbDocument(const config::Settings& settings,
                                                                    const bool refresh) {
    const auto& cache_path = settings.paths.cache_file;

    std::string cached;
    if (!refresh) {
      if (auto error = util::ReadFileText(cache_path, cached)) {
        LOG_DETAIL("no usable cache at {} ({})", cache_path.generic_string(), *error);
      } else {
        LOG_DETAIL("using cached nativedb {} ({} bytes)", cache_path.generic_string(), cached.size());
        return NativeDbDocument{.body = std::move(cached), .from_cache = true};
      }
    }

    LOG_DETAIL("downloading {}", settings.config.natives.url);
    const auto response = cpr::Get(
        cpr::Url{settings.config.natives.url},
        cpr::Timeout{std::chrono::milliseconds{settings.config.natives.timeout_seconds * kMillisecondsPerSecond}});

    if (response.error.code == cpr::ErrorCode::OK && response.status_code == 200) {
      LOG_DETAIL("downloaded {} bytes", response.text.size());
      if (auto error = util::WriteTextFile(cache_path, response.text, false, true)) {
        LOG_WARN("warning: cannot write nativedb cache {}: {}", cache_path.generic_string(), *error);
      }
      return NativeDbDocument{.body = response.text, .from_cache = false};
    }

    const std::string reason = response.error.code != cpr::ErrorCode::OK
                                 ? response.error.message
                                 : "HTTP " + std::to_string(response.status_code);

    if (auto error = util::ReadFileText(cache_path, cached)) {
      return std::unexpected("cannot download the native database (" + reason + ") and no cached copy exists at " +
                             cache_path.generic_string());
    }

    LOG_WARN("warning: cannot download the native database ({}); using the cached copy at {}, which may be out of date",
             reason, cache_path.generic_string());
    return NativeDbDocument{.body = std::move(cached), .from_cache = true};
  }
}
