//
// Created by X-ray on 3/6/2022.
//

#include <filesystem>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fmt/format.h>
#include "logger.hpp"
#include "../misc/common.hpp"

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
    kLOGGER = this;

    if (AttachConsole(GetCurrentProcessId()) || AllocConsole()) {
      auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
      if (!console_handle)
        throw std::runtime_error("Failed to get console handle");

      SetConsoleTitleA("GTAV Base");
      SetConsoleOutputCP(CP_UTF8);

      SetConsoleMode(console_handle);

      auto log_file_path = common::GetLogFile();
      if (std::filesystem::exists(log_file_path)) {
        auto log_file_path_tmp = log_file_path.parent_path() / fmt::format("{}_{}_crashed{}", common::GetEpoch(), log_file_path.stem().string(), log_file_path.extension().string());
        std::filesystem::rename(log_file_path, log_file_path_tmp);
        SaveLogFile(log_file_path_tmp);
      }

      cout_ = std::make_shared<std::ofstream>("CONOUT$");
      file_out_ = std::make_shared<std::ofstream>(log_file_path, std::ios_base::out | std::ios_base::app);

      if (cout_->is_open())
        *cout_ << "Console logging stream open" << std::endl;
      if (file_out_->is_open())
        *file_out_ << "File logging stream open" << std::endl;

      log_worker_->addSink(std::make_unique<LogSink>(), &LogSink::Log);
      g3::initializeLogging(log_worker_.get());
    } else {
      throw std::runtime_error("Failed to create console");
    }
  }

  Logger::~Logger() {
    log_worker_.reset();

    *file_out_ << "File logging stream closing" << std::endl;
    file_out_->close();
    try {
      auto log_file = common::GetLogFile();
      if (std::filesystem::exists(log_file)) {
        auto log_file_tmp = log_file.parent_path() / fmt::format("{}_{}{}", common::GetEpoch(), log_file.stem().string(), log_file.extension().string());
        std::filesystem::rename(log_file, log_file_tmp);
        SaveLogFile(log_file_tmp);
      }
    } catch (std::filesystem::filesystem_error& e) {
      *cout_ << "Exception while storing log file: " << e.what() << '\n' << "src: " << e.path1().string() << '\n' << "dst: " << e.path2().string() << '\n';
    }

    *cout_ << "Console logging stream closing" << std::endl;
    cout_->close();
    FreeConsole();
    kLOGGER = nullptr;
  }

  void Logger::LogSink::Log(g3::LogMessageMover log) {
    if (kLOGGER->GetCout()->is_open())
      *kLOGGER->GetCout() << log.get().toString(FormatConsole) << std::flush;
    if (kLOGGER->GetFileOut()->is_open())
      *kLOGGER->GetFileOut() << log.get().toString(FormatFile) << std::flush;
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