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
#include "stacktrace.hpp"

namespace gta_base {
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

  Logger::Logger() {
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

      spdlog::init_thread_pool(spdlog::details::default_async_q_size, 1);

      auto console_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      auto file_logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path.string());

      std::vector<spdlog::sink_ptr> sinks {console_logger, file_logger};
      auto logger = std::make_shared<spdlog::async_logger>("main_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
      logger->set_pattern("[%T] [%^%l%$] [Thread: %t] [%s:%#] %v");
      logger->set_level(spdlog::level::trace);
      spdlog::register_logger(logger);
      spdlog::set_default_logger(logger);

      SetupExceptionHandler();

      kLOGGER = this;
    } else {
      throw std::runtime_error("Failed to create console");
    }
  }

  Logger::~Logger() {
    Shutdown();
  }

  void Logger::Shutdown() {
    kLOGGER = nullptr;

    RemoveExceptionHandler();

    spdlog::default_logger_raw()->flush();
    spdlog::drop_all();
    spdlog::shutdown();
    FreeConsole();

    try {
      auto log_file = common::GetLogFile();
      if (std::filesystem::exists(log_file)) {
        auto log_file_tmp = log_file.parent_path() / fmt::format("{}_{}{}", common::GetEpoch(), log_file.stem().string(), log_file.extension().string());
        std::filesystem::rename(log_file, log_file_tmp);
        SaveLogFile(log_file_tmp);
      }
    } catch (std::filesystem::filesystem_error& e) {
      MessageBoxA(nullptr, fmt::format("{}\n{}\n{}", e.what(), e.path1().string(), e.path2().string()).c_str(), "exception while saving log file", MB_OK);
    }
  }

  void Logger::SetupExceptionHandler() {
    auto handler = [](PEXCEPTION_POINTERS except) -> LONG {
      auto err_code = except->ExceptionRecord->ExceptionCode;
      if (err_code == DBG_PRINTEXCEPTION_WIDE_C || err_code == DBG_PRINTEXCEPTION_C) {
        LOG_DEBUG("Received DBG_PRINTEXCEPTION_C ignoring");

        return EXCEPTION_CONTINUE_EXECUTION;
      } else if (logger::stacktrace::ExceptionCodeToStr(err_code) == "UNKNOWN") { // Without this the c++ try catch blocks will break
        LOG_WARN("Received unknown exception code: {}. This could be a c++ exception", err_code);

        return EXCEPTION_CONTINUE_SEARCH;
      }

      LOG_ERROR(logger::stacktrace::GetExceptionString(except));

      std::this_thread::sleep_for(std::chrono::seconds(10));
      kLOGGER->Shutdown();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      return EXCEPTION_CONTINUE_SEARCH;
    };

    vectored_exception_handler_h_ = AddVectoredExceptionHandler(false, handler);
    og_unhandled_exception_filter_h_ = SetUnhandledExceptionFilter(handler);

    if (!vectored_exception_handler_h_) {
      LOG_CRITICAL("Failed to set vectored exception handler");
    }
  }

  void Logger::RemoveExceptionHandler() {
    RemoveVectoredExceptionHandler(vectored_exception_handler_h_);
    SetUnhandledExceptionFilter(og_unhandled_exception_filter_h_);
  }
}