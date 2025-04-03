//
// Created by xray on 06/09/2023.
//

#include "vectored_handler.hpp"
#include "exception_report.hpp"
#include "util.hpp"
#include "../logger.hpp"
#include "../../win32/memory.hpp"

namespace base::menu::logging::exception {
  namespace {
    PVOID cur_handler = nullptr;

    void MSVCException(const PEXCEPTION_POINTERS except) {
      const std::uint32_t pid = GetCurrentProcessId();

      auto mod_name_res = win32::memory::GetModuleNameFromAddress(pid, except->ContextRecord->Rip);
      std::string mod_name;
      if (mod_name_res.has_value()) {
        mod_name = mod_name_res.value();
      }

      auto offset_res = win32::memory::GetModuleOffsetFromAddress(pid, except->ContextRecord->Rip);
      std::uintptr_t offset = 0;
      if (offset_res.has_value()) {
        offset = offset_res.value();
      }

      const auto* exception = std::bit_cast<std::exception*>(except->ExceptionRecord->ExceptionInformation[1]);
      if (exception && exception->what()) {
        LOG_ERROR("{}+{}: {}", mod_name, offset, exception->what());
      } else {
        LOG_ERROR("{}+{}: cpp exception thrown", mod_name, offset);
      }
    }

    LONG VectoredExceptionHandler(const PEXCEPTION_POINTERS except) {
      auto err_code = except->ExceptionRecord->ExceptionCode;

      if (err_code == EXCEPTION_BREAKPOINT || err_code == EXCEPTION_SINGLE_STEP) {
        return EXCEPTION_CONTINUE_SEARCH;
      }

      if (ExceptionCodeToStr(err_code) == "UNKNOWN") {
        // check for output debug string
        if (err_code != 1073807366) {
          LOG_DEBUG("Ignoring vectored exception call with code: {}", err_code);
        }

        return EXCEPTION_CONTINUE_SEARCH;
      }

      if (err_code == 3765269347 && except->ExceptionRecord->NumberParameters >= 3) {
        MSVCException(except);

        return EXCEPTION_CONTINUE_SEARCH;
      }

      // TODO: implement some cursed fatal exception recovery

      auto stacktrace_res = WriteExceptionReport(except, 7);
      if (stacktrace_res.has_value()) {
        LOG_CRITICAL(stacktrace_res.value());
      }

      spdlog::default_logger_raw()->flush();

      Manager::Shutdown();

      return EXCEPTION_CONTINUE_SEARCH;
    }
  }

  void EnableHandler() {
    cur_handler = AddVectoredExceptionHandler(false, VectoredExceptionHandler);
  }

  void DisableHandler() {
    RemoveVectoredExceptionHandler(cur_handler);
    cur_handler = nullptr;
  }
}
