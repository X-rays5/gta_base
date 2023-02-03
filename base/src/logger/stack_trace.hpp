//
// Created by X-ray on 08/24/22.
//

#pragma once
#ifndef GTA_BASE_STACK_TRACE_HPP
#define GTA_BASE_STACK_TRACE_HPP
#include <string>

namespace gta_base::logger::stacktrace {
  std::string ExceptionCodeToStr(std::uint32_t code);

  std::string GetExceptionString(PEXCEPTION_POINTERS except, std::size_t stacktrace_skip_count);
  std::string GetExceptionString(PEXCEPTION_RECORD except_rec, PCONTEXT ctx, std::size_t stacktrace_skip_count);
}
#endif //GTA_BASE_STACK_TRACE_HPP
