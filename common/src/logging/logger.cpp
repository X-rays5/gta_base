//
// Created by X-ray on 05/09/2023.
//

#include "logger.hpp"
#include <filesystem>
#include <vector>
#include <xorstr.hpp>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "../globals.hpp"
#include "../vfs.hpp"
#include "../util/time.hpp"
#include "exception/vectored_handler.hpp"
#include "formatter/thread_id.hpp"

namespace base::common::logging {
  namespace {
    bool LOGGER_EXISTS = false;

    bool SetConsoleMode(const HANDLE console_handle) {
      DWORD console_mode{};
      if (!GetConsoleMode(console_handle, &console_mode)) {
        if (GetLastError() == 6) {
          // We are most likely running in an ide so ignore this.
          return true;
        }

        MessageBox(nullptr, win32::GetLastErrorStr().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return false;
      }

      console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      console_mode &= ~ENABLE_QUICK_EDIT_MODE;

      return ::SetConsoleMode(console_handle, console_mode);
    }

    bool EnsureConsole() {
      if (AttachConsole(GetCurrentProcessId()) || AllocConsole()) {
        const HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console_handle == INVALID_HANDLE_VALUE) {
          MessageBoxA(nullptr, xorstr_("Failed to get console handle."), xorstr_("Critical logging error"), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
          return false;
        }

        SetConsoleTitleA(globals::kBASE_NAME);
        SetConsoleOutputCP(CP_UTF8);

        return SetConsoleMode(console_handle);
      }

      return false;
    }

    void SaveLogFile(const std::filesystem::path& path) {
      const auto save_path = vfs::GetLoggingSaveDir() / path.filename();
      std::filesystem::rename(path, save_path);
    }

    std::filesystem::path GetLogFile() {
      static const auto log_file = std::string(globals::kBASE_NAME) + ".log";

      return vfs::GetLoggingDir() / log_file;
    }

    void MovePossibleCrashLog() {
      const auto log_file_path = GetLogFile();
      if (std::filesystem::exists(log_file_path)) {
        const auto log_file_path_tmp = log_file_path.parent_path() / fmt::format("{}_{}_hard_crash{}", util::time::GetTimeStamp(), log_file_path.stem().string(), log_file_path.extension().string());
        std::filesystem::rename(log_file_path, log_file_path_tmp);
        SaveLogFile(log_file_path_tmp);
      }
    }

    std::shared_ptr<spdlog::async_logger> SetupLoggerInst(const std::string& logger_name) {
      const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(GetLogFile().string());

      std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
      auto logger = std::make_shared<spdlog::async_logger>(logger_name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

      logger->set_level(spdlog::level::trace);

      spdlog::register_logger(logger);

      auto formatter = std::make_unique<spdlog::pattern_formatter>();
      formatter->add_flag<ThreadIdFormatter>('N').set_pattern("[%T] [%^%l%$] [thread: %N] [%s:%#] %v");
      spdlog::set_formatter(std::move(formatter));

      logger->set_error_handler([&](const std::string& err) {
        logger->critical("spdlog: {}", err);
      });

      return logger;
    }
  }

  Manager::Manager() {
    if (LOGGER_EXISTS) {
      MessageBox(nullptr, xorstr_("Logger already exists."), xorstr_("Critical logging error"), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
      abort();
    }

    LOGGER_EXISTS = true;
    Init();
  }

  Manager::~Manager() {
    Shutdown();
    LOGGER_EXISTS = false;
  }

  void Manager::Init() {
    if (!EnsureConsole()) {
      MessageBoxA(nullptr, xorstr_("There was an error creating/obtaining a console window."), xorstr_("Critical logging error"), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
      abort();
    }

    MovePossibleCrashLog();

    spdlog::init_thread_pool(spdlog::details::default_async_q_size, 2);

    const auto logger = SetupLoggerInst(fmt::format("{}_main_logger", common::globals::kBASE_NAME));
    spdlog::set_default_logger(logger);

    exception::EnableHandler();
  }

  void Manager::Shutdown() noexcept {
    // We're already dead
    if (!spdlog::default_logger_raw())
      return;

    exception::DisableHandler();

    spdlog::default_logger_raw()->flush();
    spdlog::drop_all();
    spdlog::shutdown();

    FreeConsole();

    try {
      if (const auto log_file = GetLogFile(); std::filesystem::exists(log_file)) {
        const auto log_file_tmp = log_file.parent_path() / fmt::format("{}_{}{}", common::util::time::GetTimeStamp(), log_file.stem().string(), log_file.extension().string());
        std::filesystem::rename(log_file, log_file_tmp);

        SaveLogFile(log_file_tmp);
      }
    } catch (std::filesystem::filesystem_error& e) {
      MessageBoxA(nullptr, fmt::format("{}\n{}\n{}", e.what(), e.path1().string(), e.path2().string()).c_str(), "exception while saving log file", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
    }
  }
}
