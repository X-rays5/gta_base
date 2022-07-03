//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_LOGGER_HPP
#define GTABASE_LOGGER_HPP
#include <memory>
#include <spdlog/spdlog.h>

namespace gta_base {
  class Logger {
  public:
    Logger();
    ~Logger();

    template<typename... Args>
    __forceinline void trace(Args&&... args) {
      console_log_->trace(args...);
      file_log_->trace(args...);
    }

    template<typename... Args>
    __forceinline void debug(Args&&... args) {
      console_log_->debug(args...);
      file_log_->debug(args...);
    }

    template<typename... Args>
    __forceinline void info(Args&&... args) {
      console_log_->info(args...);
      file_log_->info(args...);
    }

    template<typename... Args>
    __forceinline void warn(Args&&... args) {
      console_log_->warn(args...);
      file_log_->warn(args...);
    }

    template<typename... Args>
    __forceinline void error(Args&&... args) {
      console_log_->error(args...);
      file_log_->error(args...);
    }

    template<typename... Args>
    __forceinline void critical(Args&&... args) {
      console_log_->critical(args...);
      file_log_->critical(args...);
    }

  private:
    std::shared_ptr<spdlog::logger> console_log_;
    std::shared_ptr<spdlog::logger> file_log_;
  };
  inline std::unique_ptr<Logger> kLOGGER;
}

#define LOG_TRACE(...) ::gta_base::kLOGGER->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::gta_base::kLOGGER->debug(__VA_ARGS__)
#define LOG_INFO(...) ::gta_base::kLOGGER->info(__VA_ARGS__)
#define LOG_WARN(...) ::gta_base::kLOGGER->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::gta_base::kLOGGER->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::gta_base::kLOGGER->critical(__VA_ARGS__)
#define LOG_FATAL(...) ::gta_base::kLOGGER->fatal(__VA_ARGS__)
#endif //GTABASE_LOGGER_HPP
