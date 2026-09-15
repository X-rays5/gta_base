#include "fetch.hpp"

#include <chrono>
#include <future>
#include <system_error>

// glaze/net headers use local 'version' variables that trip MSVC C4459 ("declaration of
// 'version' hides global declaration") against glaze/version.hpp's glz::version, which this
// tool treats as an error. The suppression is scoped to parsing the third-party headers only.
#pragma warning(push)
#pragma warning(disable : 4459)
#include <glaze/net/http_client.hpp>
#pragma warning(pop)

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

    const auto fallback_to_cache = [&](const std::string& reason) -> std::expected<NativeDbDocument, std::string> {
      if (auto error = util::ReadFileText(cache_path, cached)) {
        return std::unexpected("cannot download the native database (" + reason + ") and no cached copy exists at " +
                               cache_path.generic_string());
      }

      LOG_WARN("warning: cannot download the native database ({}); using the cached copy at {}, which may be out of date",
               reason, cache_path.generic_string());
      return NativeDbDocument{.body = std::move(cached), .from_cache = true};
    };

    LOG_DETAIL("downloading {}", settings.config.natives.url);

    glz::http_client client{};
    std::future<std::expected<glz::response, std::error_code>> request = client.get_async(settings.config.natives.url);

    const auto timeout = std::chrono::milliseconds{settings.config.natives.timeout_seconds * kMillisecondsPerSecond};
    if (request.wait_for(timeout) != std::future_status::ready) {
      return fallback_to_cache("timed out after " + std::to_string(settings.config.natives.timeout_seconds) +
                               " seconds");
    }

    auto result = request.get();
    if (!result) {
      return fallback_to_cache(result.error().message());
    }

    const glz::response& response = *result;
    if (response.status_code == 200) {
      LOG_DETAIL("downloaded {} bytes", response.response_body.size());
      if (auto error = util::WriteTextFile(cache_path, response.response_body, false, true)) {
        LOG_WARN("warning: cannot write nativedb cache {}: {}", cache_path.generic_string(), *error);
      }
      return NativeDbDocument{.body = response.response_body, .from_cache = false};
    }

    return fallback_to_cache("HTTP " + std::to_string(response.status_code));
  }
}
