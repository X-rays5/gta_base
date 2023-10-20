//
// Created by xray on 06/09/2023.
//

#include "stacktrace.hpp"
#include <stacktrace>
#include "../../util/common.hpp"
#include "util.hpp"

namespace base::logging::exception {
  namespace {
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
      res << "Rip: " << util::common::AddrToHex(ctx->Rip) << "\n";
      res << "Rax: " << util::common::AddrToHex(ctx->Rax) << "\n";
      res << "Rbx: " << util::common::AddrToHex(ctx->Rbx) << "\n";
      res << "Rcx: " << util::common::AddrToHex(ctx->Rcx) << "\n";
      res << "Rdx: " << util::common::AddrToHex(ctx->Rdx) << "\n";
      res << "Rsi: " << util::common::AddrToHex(ctx->Rsi) << "\n";
      res << "Rdi: " << util::common::AddrToHex(ctx->Rdi) << "\n";
      res << "RSp: " << util::common::AddrToHex(ctx->Rsp) << "\n";
      res << "Rbp: " << util::common::AddrToHex(ctx->Rbp) << "\n";
      res << "R8: " << util::common::AddrToHex(ctx->R8) << "\n";
      res << "R9: " << util::common::AddrToHex(ctx->R9) << "\n";
      res << "R10: " << util::common::AddrToHex(ctx->R10) << "\n";
      res << "R11: " << util::common::AddrToHex(ctx->R11) << "\n";
      res << "R12: " << util::common::AddrToHex(ctx->R12) << "\n";
      res << "R13: " << util::common::AddrToHex(ctx->R13) << "\n";
      res << "R14: " << util::common::AddrToHex(ctx->R14) << "\n";
      res << "R15: " << util::common::AddrToHex(ctx->R15) << "\n";

      return res.str();
    }
  }

  absl::StatusOr<std::string> GetStackTrace(PEXCEPTION_RECORD except_rec, PCONTEXT ctx, std::size_t stacktrace_skip_count) {
    #ifndef NDEBUG
    //__debugbreak();
    #endif

    std::stringstream msg;

    msg << "\n***** EXCEPTION RECEIVED *****\n\n";
    msg << "***** Exception name: " << ExceptionCodeToStr(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception code: " << util::common::AddrToHex(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception pid: " << GetCurrentProcessId() << " *****\n";

    auto except_mod_res = win32::memory::GetModuleNameFromAddress(GetCurrentProcessId(), ctx->Rip);
    if (!except_mod_res.ok())
      return except_mod_res.status();

    msg << "***** Exception module: " << except_mod_res.value() << " *****\n";
    msg << "***** Exception address: 0x" << except_rec->ExceptionAddress << " *****\n";
    //msg << "***** Exception instruction: " << common::GetInstructionStr((std::uintptr_t) except_rec->ExceptionAddress) << " *****\n"; TODO: impl zydis decomp
    msg << "***** Exception flags: " << except_rec->ExceptionFlags << " *****\n";
    msg << "\n***** STACKDUMP *****\n";

    msg << "\nLoaded Modules:\n";

    auto mod_res = win32::GetProcessModules(GetCurrentProcessId());
    if (!mod_res.ok())
      return mod_res.status();

    std::vector<MODULEENTRY32> modules = mod_res.value();
    for (auto&& mod : modules) {
      msg << mod.szExePath << " addr: " << util::common::AddrToHex(reinterpret_cast<std::uintptr_t>(mod.modBaseAddr)) << " size: " << util::common::AddrToHex(mod.modBaseSize);
      msg << '\n';
    }

    msg << GetRegisters(ctx);

    msg << '\n' << std::stacktrace::current(stacktrace_skip_count) << '\n';

    return RemoveDoubleSpaces(msg.str());
  }
}