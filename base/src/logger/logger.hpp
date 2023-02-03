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
      static const std::size_t MAX_REPEATING_EXCEPTIONS = 5;
      static const std::uint64_t MAX_REPEATING_EXCEPTIONS_TIME = 10; // ms
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

    static void MSVCException(PEXCEPTION_POINTERS except);

  private:
    robin_hood::unordered_map<std::thread::id, ThreadExceptionInfo> thread_exception_info_;
    PVOID vectored_exception_handler_h_{};
  private:

    void Shutdown();
    void SetupExceptionHandler();

    void RemoveExceptionHandler();
  };

  inline Logger* kLOGGER{};
}

#define LOGGER_LOG(logger, level, ...) (logger)->log(spdlog::source_loc{__FILE__, __LINE__, "redacted"}, level, __VA_ARGS__)

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define LOG_TRACE(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::trace, __VA_ARGS__)
#else
#define LOG_TRACE(...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LOG_DEBUG(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define LOG_INFO(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::info, __VA_ARGS__)
#else
#define LOG_INFO(...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define LOG_WARN(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::warn, __VA_ARGS__)
#else
#define LOG_WARN(...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define LOG_ERROR(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::err, __VA_ARGS__)
#else
#define LOG_ERROR(...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define LOG_CRITICAL(...) LOGGER_LOG(spdlog::default_logger_raw(), spdlog::level::critical, __VA_ARGS__)
#else
#define LOG_CRITICAL(...)  (void)0
#endif

#endif //GTABASE_LOGGER_HPP
