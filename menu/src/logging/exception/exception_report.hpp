//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_STACKTRACE_A38C3A72B50442F086F82164348159CC_HPP
#define GTA_BASE_STACKTRACE_A38C3A72B50442F086F82164348159CC_HPP
#include <string>

namespace base::menu::logging::exception {
  StatusOr<std::string> WriteExceptionReport(PEXCEPTION_RECORD except_rec, PCONTEXT ctx, std::size_t stacktrace_skip_count);
  FORCE_INLINE StatusOr<std::string> WriteExceptionReport(PEXCEPTION_POINTERS except, std::size_t stacktrace_skip_count) {
    return WriteExceptionReport(except->ExceptionRecord, except->ContextRecord, stacktrace_skip_count);
  }
}

#endif //GTA_BASE_STACKTRACE_A38C3A72B50442F086F82164348159CC_HPP
