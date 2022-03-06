//
// Created by X-ray on 3/6/2022.
//

#include <filesystem>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "logger.hpp"
#include "../common/common.hpp"

namespace gta_base {
  Logger::Logger() {
    if (AttachConsole(GetCurrentProcessId()) || AllocConsole()) {
      auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
      if (!console_handle)
        throw std::runtime_error("Failed to get console handle");

      SetConsoleTitleA("Gta Base");
      SetConsoleOutputCP(CP_UTF8);

#if !defined(NDEBUG)
      spdlog::set_level(spdlog::level::trace);
      spdlog::set_pattern("[%T%z] [%^%L%$] [thread %t]: %v");
#else
      spdlog::set_level(spdlog::level::info);
      spdlog::set_pattern("[%T%z] [%^%l%$]: %v");
#endif
      spdlog::flush_every(std::chrono::seconds(1));

      std::string log_file_path = common::GetLogDir().string() + "/debug.log";
      if (std::filesystem::exists(log_file_path)) {
        std::filesystem::rename(log_file_path, log_file_path + ".bak");
      }

      console_log_ = spdlog::stdout_color_mt("console");
      file_log_ = spdlog::basic_logger_mt("file_log", common::GetLogDir().string() + "/debug.log");
    } else {
      throw std::runtime_error("Failed to create console");
    }
  }

  Logger::~Logger() {
    console_log_->flush();
    console_log_.reset();
    file_log_->flush();
    file_log_.reset();
    spdlog::shutdown();
    FreeConsole();

    uint64_t since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto debug_file = std::filesystem::path(common::GetLogDir().string() + "/debug.log");
    if (std::filesystem::exists(debug_file)) {
      std::filesystem::rename(debug_file, std::filesystem::path(common::GetLogSaveDir().string() + "/" + (std::to_string(since_epoch) + "_debug.log")));
    }
  }
}