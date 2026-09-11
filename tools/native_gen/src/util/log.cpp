#include "log.hpp"

#include <memory>
#include <utility>
#include <vector>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace base::tools::native_gen::util {
  void SetUpConsoleLogging(const bool quiet) {
    std::vector<spdlog::sink_ptr> sinks;
    if (const auto current = spdlog::default_logger()) {
      // Carries the Manager's file sink over to the logger this replaces.
      sinks = current->sinks();
    }

    const auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    // Only the console sink is stripped back to the bare message; the file log keeps the
    // timestamped pattern the Manager set.
    console_sink->set_pattern("%v");
    sinks.push_back(console_sink);

    // Deliberately synchronous, unlike the Manager's own logger. The Manager's pool runs two
    // worker threads off one queue, which lets the summary block's lines come out in a
    // different order on every run; this tool is single threaded and wants them in order.
    auto logger = std::make_shared<spdlog::logger>("native_gen", sinks.begin(), sinks.end());
    // Warnings and errors are all that --quiet leaves visible.
    logger->set_level(quiet ? spdlog::level::warn : spdlog::level::info);
    spdlog::set_default_logger(std::move(logger));
  }
}
