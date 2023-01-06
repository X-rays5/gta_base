//
// Created by X-ray on 3/6/2022.
//

#include <filesystem>
#include <chrono>
#include <Zydis/Zydis.h>
#include "logger.hpp"
#include "stack_trace.hpp"
#include "../misc/common.hpp"

namespace gta_base {
  void SetConsoleMode(HANDLE console_handle) {
    DWORD console_mode;
    GetConsoleMode(console_handle, &console_mode);
    console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
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
        auto log_file_path_tmp = log_file_path.parent_path() / fmt::format("{}_{}_hard_crash{}", common::GetEpoch(), log_file_path.stem().string(), log_file_path.extension().string());
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
      spdlog::default_logger_raw()->set_error_handler([](const std::string& err){
        LOG_CRITICAL(err);
      });

      SetupExceptionHandler();

      kLOGGER = this;
    } else {
      throw std::runtime_error("Failed to create console");
    }
  }

  Logger::~Logger() {
    Shutdown();
  }

  void Logger::Flush() {
    spdlog::default_logger_raw()->flush();
  }

  void Logger::RegisterThreadException(std::thread::id thread_id, std::uintptr_t address) {
    auto it = thread_exception_info_.find(thread_id);

    if (it != thread_exception_info_.end()) {
      auto* info = &it->second;

      // Check if too long since last exception
      if (info->last_exception_time + ThreadExceptionInfo::MAX_REPEATING_EXCEPTIONS_TIME < common::GetEpoch()) {
        info->last_exception_time = common::GetEpoch();
        info->exception_count = 0;
        info->last_exception_address = address;
      } else {
        info->last_exception_time = common::GetEpoch();
        info->last_exception_address = address;
        info->exception_count++;
      }
    } else {
      thread_exception_info_.emplace(thread_id, ThreadExceptionInfo{common::GetEpoch(), address, 0});
    }
  }

  bool Logger::ThreadTooManyExceptions(std::thread::id thread_id) {
    auto it = thread_exception_info_.find(thread_id);

    if (it != thread_exception_info_.end()) {
      auto* info = &it->second;
      return info->exception_count >= ThreadExceptionInfo::MAX_REPEATING_EXCEPTIONS;
    }

    return false;
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

      // ignore non fatal exceptions
      if (logger::stacktrace::ExceptionCodeToStr(err_code) == "UNKNOWN") {
        LOG_DEBUG("Ignoring vectored exception call with code: {}", err_code);
        return EXCEPTION_CONTINUE_SEARCH;
      }

      // MSVC cpp exception
      if (err_code == 3765269347) {
        auto addr = (std::uintptr_t)except->ExceptionRecord->ExceptionAddress;
        std::string file_name = common::GetModuleNameFromAddress(GetCurrentProcessId(), addr);
        auto offset = addr - (uintptr_t)GetModuleHandleA(file_name.c_str());
        auto exception = (std::exception*)except->ExceptionRecord->ExceptionInformation[1];

        if (exception) {
          LOG_ERROR("({}+0x{:X}): {}", file_name, offset, exception->what());
        } else {
          LOG_ERROR("cpp exception thrown at 0x{:X} ({}+0x{:X})", addr, file_name, offset);
        }

        // Can't try to recover here because this code runs before try catch blocks.
        // Since MSVC uses a UnhandledExceptionFilter for that.
        return EXCEPTION_CONTINUE_SEARCH;
      }

      if (!kLOGGER->ThreadTooManyExceptions(std::this_thread::get_id())) {
        auto instruction = common::GetInstructionAtAddr(except->ContextRecord->Rip);

        if (instruction.instruction.length) {
          kLOGGER->RegisterThreadException(std::this_thread::get_id(), except->ContextRecord->Rip);

          auto next_instruction = except->ContextRecord->Rip + instruction.instruction.length;
          LOG_WARN("EXCEPTION Recovery: {} ({:X}) -> {} ({:X})", common::GetInstructionStr(except->ContextRecord->Rip, instruction), except->ContextRecord->Rip, common::GetInstructionStr(next_instruction), next_instruction);
          except->ContextRecord->Rip = next_instruction;

          return EXCEPTION_CONTINUE_EXECUTION;
        }
      }

      LOG_CRITICAL("EXCEPTION Recovery: failed to recover from crash exceeded recursive exception limit");
      LOG_ERROR(logger::stacktrace::GetExceptionString(except));

      // make sure the stacktrace is saved on disk
      kLOGGER->Flush();
      globals::running = false;

      return EXCEPTION_CONTINUE_SEARCH;
    };

    vectored_exception_handler_h_ = AddVectoredExceptionHandler(false, handler);

    if (!vectored_exception_handler_h_) {
      LOG_CRITICAL("Failed to set vectored exception handler");
    }
  }

  void Logger::RemoveExceptionHandler() {
    RemoveVectoredExceptionHandler(vectored_exception_handler_h_);
  }
}