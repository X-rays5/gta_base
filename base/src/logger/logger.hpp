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
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>

#undef GetObject

namespace gta_base {
  class Logger {
  public:
    struct ThreadExceptionInfo {
      static const std::size_t MAX_REPEATING_EXCEPTIONS = 10;
      static const std::uint64_t MAX_REPEATING_EXCEPTIONS_TIME = 2; // ms
      std::uint64_t last_exception_time{};
      std::uintptr_t last_exception_address{};
      std::size_t exception_count{};
    };

  public:
    Logger();
    ~Logger();

    void Flush();

    void RegisterThreadException(std::thread::id thread_id, std::uintptr_t address);

    bool ThreadTooManyExceptions(std::thread::id thread_id);

  private:
    robin_hood::unordered_map<std::thread::id, ThreadExceptionInfo> thread_exception_info_;

    PVOID vectored_exception_handler_h_;

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
