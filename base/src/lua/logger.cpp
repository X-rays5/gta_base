//
// Created by X-ray on 02/09/23.
//

#include "logger.hpp"

#include <utility>
#include "../logger/sinks/on_screen_console.hpp"

#define LOGGER_NAME() fmt::format("{}_logger", script_name_)

namespace gta_base::lua {
  Logger::Logger(sol::state* L, std::string script_name) : script_name_(std::move(script_name)), lua_state_(L) {
    if (std::filesystem::exists (GetLogFile())) {
      SaveLogFile();
    }

    auto console_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(GetLogFile().string());
    auto on_screen_console_logger = std::make_shared<logger::sinks::OnScreenConsole_mt>();

    std::vector<spdlog::sink_ptr> sinks{console_logger, file_logger, on_screen_console_logger};
    auto logger = std::make_shared<spdlog::async_logger>(LOGGER_NAME(), sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    logger->set_pattern("[%T] [%^%l%$] [thread: %t] %v");
    logger->set_level(spdlog::level::trace);

    spdlog::register_logger(logger);

    logger_inst_ = spdlog::get(LOGGER_NAME());
  }

  Logger::~Logger() {
    spdlog::drop(LOGGER_NAME());

    // can't save the file here since spdlog will still have the lock
  }
}

#undef LOGGER_NAME