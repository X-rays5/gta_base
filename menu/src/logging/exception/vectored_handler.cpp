//
// Created by xray on 06/09/2023.
//

#include "vectored_handler.hpp"
#include "util.hpp"
#include "../logger.hpp"
#include "stacktrace.hpp"

namespace base::logging::exception {
  namespace {
    void MSVCException(PEXCEPTION_POINTERS except) {
      std::uint32_t pid = GetCurrentProcessId();

      auto mod_name = win32::memory::GetModuleNameFromAddress(pid, except->ContextRecord->Rip);
      auto offset = win32::memory::GetModuleOffsetFromAddress(pid, except->ContextRecord->Rip);
      auto exception = reinterpret_cast<std::exception*>(except->ExceptionRecord->ExceptionInformation[1]);

      if (exception && exception->what()) {
        LOG_ERROR("{}+{}: {}", mod_name, offset, exception->what());
      } else {
        LOG_ERROR("{}+{}: cpp exception thrown");
      }
    }

    LONG VectoredExceptionHandler(PEXCEPTION_POINTERS except) {
      auto err_code = except->ExceptionRecord->ExceptionCode;

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

      GetStackTrace(except, 7);

      spdlog::default_logger_raw()->flush();
      std::this_thread::sleep_for(std::chrono::seconds(10));

      logging::Manager::Shutdown();

      return EXCEPTION_CONTINUE_SEARCH;
    }

    PVOID cur_handler = nullptr;
  }

  void EnableHandler() {
    cur_handler = AddVectoredExceptionHandler(false, VectoredExceptionHandler);
  }

  void DisableHandler() {
    RemoveVectoredExceptionHandler(cur_handler);
    cur_handler = nullptr;
  }
}