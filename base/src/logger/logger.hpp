//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_LOGGER_HPP
#define GTABASE_LOGGER_HPP
#include <memory>
#include <fstream>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/logmessage.hpp>
#include <fmt/format.h>

namespace gta_base {
  class Logger {
  public:
    Logger();
    ~Logger();

  private:
    enum class LogColor {
      RESET,
      WHITE = 97,
      CYAN = 36,
      MAGENTA = 35,
      BLUE = 34,
      GREEN = 32,
      YELLOW = 33,
      RED = 31,
      BLACK = 30
    };

    struct LogSink {
      void Log(g3::LogMessageMover log);
      static LogColor GetColor(const LEVELS& level);
      static std::string FormatConsole(const g3::LogMessage& msg);
      static std::string FormatFile(const g3::LogMessage& msg);
    };

    std::unique_ptr<g3::LogWorker> log_worker_;
    std::ofstream cout_;
    std::ofstream file_out_;
  };
  inline std::unique_ptr<Logger> kLOGGER;
}

#define LOG_DEBUG LOG(DEBUG)
#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_FATAL LOG(FATAL)
#endif //GTABASE_LOGGER_HPP
