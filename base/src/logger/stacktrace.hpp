//
// Created by X-ray on 08/24/22.
//

#pragma once
#ifndef GTA_BASE_STACKTRACE_HPP
#define GTA_BASE_STACKTRACE_HPP
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace gta_base {
  namespace logger {
    namespace stacktrace {
      std::string GetExceptionString(PEXCEPTION_POINTERS except);
      std::string GetExceptionString(PEXCEPTION_RECORD except_rec, PCONTEXT ctx);
    }
  }
}
#endif //GTA_BASE_STACKTRACE_HPP