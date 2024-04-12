//
// Created by xray on 06/09/2023.
//

#include "stacktrace.hpp"
#include <stacktrace>
#include <Zydis/Zydis.h>
#include "../../util/common.hpp"
#include "util.hpp"
#include "../../memory/address.hpp"

namespace base::logging::exception {
  namespace {
    StatusOr<std::string> GetInstructionsStr(const std::uintptr_t addr) {
      const std::uint32_t pid = GetCurrentProcessId();
      auto except_mod_res = win32::memory::GetModuleFromAddress(pid, addr);
      if (except_mod_res.error())
        return except_mod_res.error().Forward();

      const std::uintptr_t offset = addr - reinterpret_cast<std::uintptr_t>(except_mod_res.value().modBaseAddr);

      auto mem_info_res = memory::Address(addr).GetMemoryInformation();
      if (!mem_info_res) {
        LOG_CRITICAL("Failed to get memory information for exception at: 0x{:X}", addr);
      }
      MEMORY_BASIC_INFORMATION mem_info = mem_info_res.value();

      if (mem_info.AllocationProtect != PAGE_EXECUTE_READ && mem_info.AllocationProtect != PAGE_EXECUTE_READWRITE && mem_info.AllocationProtect != PAGE_EXECUTE_WRITECOPY) {
        LOG_ERROR("Address is not in executable memory.");
        LOG_DEBUG("{}+0x{:X} NO EXECUTE", except_mod_res.value().szModule, offset);
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Address is not in executable memory.");
      }

      ZydisDisassembledInstruction instruction;
      if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, addr, reinterpret_cast<void*>(addr), 15, &instruction))) {
        LOG_ERROR("Failed to disassemble exception area.");
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to disassemble exception area.");
      }

      return fmt::format("{}+0x{:X}\t{}", except_mod_res.value().szModule, offset, instruction.text);
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

  StatusOr<std::string> GetStackTrace(PEXCEPTION_RECORD except_rec, PCONTEXT ctx, std::size_t stacktrace_skip_count) {
#ifndef NDEBUG
    //__debugbreak();
#endif

    std::stringstream msg;

    msg << "\n***** EXCEPTION RECEIVED *****\n\n";
    msg << "***** Exception name: " << ExceptionCodeToStr(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception code: " << util::common::AddrToHex(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception pid: " << GetCurrentProcessId() << " *****\n";

    auto except_mod_res = win32::memory::GetModuleNameFromAddress(GetCurrentProcessId(), ctx->Rip);
    if (except_mod_res.error())
      return except_mod_res.error().Forward();

    msg << "***** Exception module: " << except_mod_res.value() << " *****\n";
    msg << "***** Exception address: 0x" << except_rec->ExceptionAddress << " *****\n";
    msg << "***** Exception flags: " << except_rec->ExceptionFlags << " *****\n";
    msg << "***** Exception instruction: " << GetInstructionsStr(ctx->Rip).value_or("Failed to decompile exception area.") << " *****\n";

    msg << "\n***** STACKDUMP *****\n";

    msg << "\nLoaded Modules:\n";

    auto mod_res = win32::GetProcessModules(GetCurrentProcessId());
    if (mod_res.error())
      return mod_res.error().Forward();

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
