//
// Created by X-ray on 3/6/2022.
//

#include <filesystem>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fmt/format.h>
#include "logger.hpp"
#include "../common/common.hpp"

namespace gta_base {
  #define AddColorToStream(color) "\x1b[" << int(color) << "m"
  #define ResetStreamColor "\x1b[" << int(LogColor::RESET) << "m"

  void SetConsoleMode(HANDLE console_handle) {
    DWORD console_mode;
    GetConsoleMode(console_handle, &console_mode);
    console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    console_mode &= ~(ENABLE_QUICK_EDIT_MODE);

    ::SetConsoleMode(console_handle, console_mode);
  }

  void SaveLogFile(const std::filesystem::path& path) {
    auto save_path = common::GetLogSaveDir() / path.filename();
    std::filesystem::rename(path, save_path);
  }

  Logger::Logger() : log_worker_(g3::LogWorker::createLogWorker()) {
    if (AttachConsole(GetCurrentProcessId()) || AllocConsole()) {
      auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
      if (!console_handle)
        throw std::runtime_error("Failed to get console handle");

      SetConsoleTitleA("GTAV Base");
      SetConsoleOutputCP(CP_UTF8);

      SetConsoleMode(console_handle);

      auto log_file_path = common::GetLogDir() / fmt::format("{}.log", common::globals::name);
      if (std::filesystem::exists(log_file_path)) {
        uint64_t since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto log_file_path_tmp = log_file_path.parent_path() / fmt::format("{}_{}_crashed{}", since_epoch, log_file_path.stem().string(), log_file_path.extension().string());
        std::filesystem::rename(log_file_path, log_file_path_tmp);
        SaveLogFile(log_file_path_tmp);
      }

      cout_.open("CONOUT$", std::ios_base::out | std::ios_base::app);
      file_out_.open(log_file_path, std::ios_base::out | std::ios_base::trunc);

      log_worker_->addSink(std::make_unique<LogSink>(), &LogSink::Log);
      g3::initializeLogging(log_worker_.get());
    } else {
      throw std::runtime_error("Failed to create console");
    }
  }

  Logger::~Logger() {
    log_worker_->removeAllSinks();
    log_worker_.reset();

    if (cout_.is_open())
      cout_.close();
    if (file_out_.is_open())
      file_out_.close();

    FreeConsole();

    uint64_t since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto debug_file = std::filesystem::path(common::GetLogDir().string() + fmt::format("/{}.log", common::globals::name));
    if (std::filesystem::exists(debug_file)) {
      std::filesystem::rename(debug_file, std::filesystem::path(common::GetLogSaveDir().string() + fmt::format("/{}_{}.log", since_epoch, common::globals::name)));
    }
  }

  void Logger::LogSink::Log(g3::LogMessageMover log) {
    if (kLOGGER->cout_.is_open())
      kLOGGER->cout_ << log.get().toString(FormatConsole) << std::flush;
    if (kLOGGER->file_out_.is_open())
      kLOGGER->file_out_ << log.get().toString(FormatFile) << std::flush;
  }

  Logger::LogColor Logger::LogSink::GetColor(const LEVELS& level) {
    switch (level.value) {
      case g3::kDebugValue:
        return LogColor::BLUE;
      case g3::kInfoValue:
        return LogColor::GREEN;
      case g3::kWarningValue:
        return LogColor::YELLOW;
    }
    return g3::internal::wasFatal(level) ? LogColor::RED : LogColor::WHITE;
  }

  std::string Logger::LogSink::FormatConsole(const g3::LogMessage& msg) {
    LogColor color = GetColor(msg._level);
    std::stringstream color_start;
    std::stringstream color_end;
    color_start << AddColorToStream(color);
    color_end << ResetStreamColor;

    return fmt::format("[{}] [{}{}{}] [Thread: {}] [{}:{}] ", msg.timestamp("%H:%M:%S"), color_start.str(), msg.level(), color_end.str(), msg.threadID(), msg.file(), msg.line());
  }

  std::string Logger::LogSink::FormatFile(const g3::LogMessage& msg) {
    return fmt::format("[{}] [{}] [Thread: {}] [{}:{}] ", msg.timestamp("%H:%M:%S"), msg.level(), msg.threadID(), msg.file(), msg.line());
  }
}