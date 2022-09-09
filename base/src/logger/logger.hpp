//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_LOGGER_HPP
#define GTABASE_LOGGER_HPP

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL 1 // DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL 2 // INFO
#endif

#include <memory>
#include <iostream>
#include <fstream>
#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef GetObject

namespace gta_base {
  class Logger {
  public:
    Logger();
    ~Logger();

  private:
    PVOID vectored_exception_handler_h_;
    LPTOP_LEVEL_EXCEPTION_FILTER  og_unhandled_exception_filter_h_;

  private:
    void Shutdown();

    void SetupExceptionHandler();
    void RemoveExceptionHandler();
  };
  inline Logger* kLOGGER{};
}

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)  SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define LOG_FATAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#endif //GTABASE_LOGGER_HPP
