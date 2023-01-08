//
// Created by X-ray on 08/24/22.
//

#pragma once
#ifndef GTA_BASE_STACK_TRACE_HPP
#define GTA_BASE_STACK_TRACE_HPP
#include <string>

namespace gta_base::logger::stacktrace {
  extern std::uint32_t RECURSIVE_CRASH_CHECK;

  std::string ExceptionCodeToStr(std::uint32_t code);
  std::string GetFileExceptionOccured(PEXCEPTION_RECORD except_rec);

  std::string GetExceptionString(PEXCEPTION_POINTERS except);
  std::string GetExceptionString(PEXCEPTION_RECORD except_rec, PCONTEXT ctx);
}
#endif //GTA_BASE_STACK_TRACE_HPP
