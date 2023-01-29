//
// Created by X-ray on 08/24/22.
//

#include "stack_trace.hpp"
#include <sstream>
#include <stacktrace>

#define STACKTRACE_SKIP_FUNC_COUNT 7

namespace gta_base::logger::stacktrace {

  std::string ExceptionCodeToStr(std::uint32_t code) {
    switch (code) {
      case EXCEPTION_ACCESS_VIOLATION:
        return "EXCEPTION_ACCESS_VIOLATION";
      case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "EXCEPTION_DATATYPE_MISALIGNMENT";
      case EXCEPTION_BREAKPOINT:
        return "EXCEPTION_BREAKPOINT";
      case EXCEPTION_SINGLE_STEP:
        return "EXCEPTION_SINGLE_STEP";
      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
      case EXCEPTION_FLT_DENORMAL_OPERAND:
        return "EXCEPTION_FLT_DENORMAL_OPERAND";
      case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
      case EXCEPTION_FLT_INEXACT_RESULT:
        return "EXCEPTION_FLT_INEXACT_RESULT";
      case EXCEPTION_FLT_INVALID_OPERATION:
        return "EXCEPTION_FLT_INVALID_OPERATION";
      case EXCEPTION_FLT_OVERFLOW:
        return "EXCEPTION_FLT_OVERFLOW";
      case EXCEPTION_FLT_STACK_CHECK:
        return "EXCEPTION_FLT_STACK_CHECK";
      case EXCEPTION_FLT_UNDERFLOW:
        return "EXCEPTION_FLT_UNDERFLOW";
      case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "EXCEPTION_INT_DIVIDE_BY_ZERO";
      case EXCEPTION_INT_OVERFLOW:
        return "EXCEPTION_INT_OVERFLOW";
      case EXCEPTION_PRIV_INSTRUCTION:
        return "EXCEPTION_PRIV_INSTRUCTION";
      case EXCEPTION_IN_PAGE_ERROR:
        return "EXCEPTION_IN_PAGE_ERROR";
      case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "EXCEPTION_ILLEGAL_INSTRUCTION";
      case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
      case EXCEPTION_STACK_OVERFLOW:
        return "EXCEPTION_STACK_OVERFLOW";
      case EXCEPTION_INVALID_DISPOSITION:
        return "EXCEPTION_INVALID_DISPOSITION";
      case EXCEPTION_GUARD_PAGE:
        return "EXCEPTION_GUARD_PAGE";
      case EXCEPTION_INVALID_HANDLE:
        return "EXCEPTION_INVALID_HANDLE";
      case 3765269347:
        return "EXCEPTION_MSVC_CPP";
      default:
        return "UNKNOWN";
    }
  }

  std::string RemoveDoubleSpaces(const std::string& str) {
    std::string result;
    for (auto it = str.begin(); it != str.end(); ++it) {
      if (it != str.begin() && *it == ' ' && *(it - 1) == ' ')
        continue;
      result.push_back(*it);
    }
    return result;
  }

  std::string GetRegisters(PCONTEXT ctx) {
    std::stringstream res;
    res << "\nDumping registers:\n";
    res << "Rip: " << common::AddrToHex(ctx->Rip) << "\n";
    res << "Rax: " << common::AddrToHex(ctx->Rax) << "\n";
    res << "Rbx: " << common::AddrToHex(ctx->Rbx) << "\n";
    res << "Rcx: " << common::AddrToHex(ctx->Rcx) << "\n";
    res << "Rdx: " << common::AddrToHex(ctx->Rdx) << "\n";
    res << "Rsi: " << common::AddrToHex(ctx->Rsi) << "\n";
    res << "Rdi: " << common::AddrToHex(ctx->Rdi) << "\n";
    res << "RSp: " << common::AddrToHex(ctx->Rsp) << "\n";
    res << "Rbp: " << common::AddrToHex(ctx->Rbp) << "\n";
    res << "R8: " << common::AddrToHex(ctx->R8) << "\n";
    res << "R9: " << common::AddrToHex(ctx->R9) << "\n";
    res << "R10: " << common::AddrToHex(ctx->R10) << "\n";
    res << "R11: " << common::AddrToHex(ctx->R11) << "\n";
    res << "R12: " << common::AddrToHex(ctx->R12) << "\n";
    res << "R13: " << common::AddrToHex(ctx->R13) << "\n";
    res << "R14: " << common::AddrToHex(ctx->R14) << "\n";
    res << "R15: " << common::AddrToHex(ctx->R15) << "\n";

    return res.str();
  }

  std::string GetExceptionString(PEXCEPTION_POINTERS except) {
    return GetExceptionString(except->ExceptionRecord, except->ContextRecord);
  }

  std::string GetExceptionString(PEXCEPTION_RECORD except_rec, PCONTEXT ctx) {
    #ifndef NDEBUG
    //__debugbreak();
    #endif

    std::stringstream msg;

    msg << "\n***** EXCEPTION RECEIVED *****\n\n";
    msg << "***** Exception name: " << ExceptionCodeToStr(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception code: " << common::AddrToHex(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception pid: " << GetCurrentProcessId() << " *****\n";
    msg << "***** Exception module: " << common::GetModuleNameFromAddress(GetCurrentProcessId(), ctx->Rip) << " *****\n";
    msg << "***** Exception address: 0x" << except_rec->ExceptionAddress << " *****\n";
    msg << "***** Exception instruction: " << common::GetInstructionStr((std::uintptr_t) except_rec->ExceptionAddress) << " *****\n";
    msg << "***** Exception flags: " << except_rec->ExceptionFlags << " *****\n";
    msg << "\n***** STACKDUMP *****\n";

    msg << "\nLoaded Modules:\n";
    std::vector<MODULEENTRY32> modules = common::GetProcessModules(GetCurrentProcessId());
    for (auto&& mod : modules) {
      msg << mod.szExePath << " addr: " << common::AddrToHex(reinterpret_cast<std::uintptr_t>(mod.modBaseAddr)) << " size: " << common::AddrToHex(mod.modBaseSize);
      msg << '\n';
    }

    msg << GetRegisters(ctx);

    msg << '\n' << std::stacktrace::current(STACKTRACE_SKIP_FUNC_COUNT) << '\n';

    return RemoveDoubleSpaces(msg.str());
  }
}