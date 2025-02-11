//
// Created by xray on 06/09/2023.
//

#include "exception_report.hpp"
#include <stacktrace>
#include <fstream>
#include <Zydis/Zydis.h>
#include <DbgHelp.h>
#include <base-common/conversion.hpp>
#include <base-common/time.hpp>
#include "util.hpp"
#include "../../memory/address.hpp"
#include "../../util/vfs.hpp"

namespace base::logging::exception {
  namespace {
    Status WriteMiniDump(std::filesystem::path path, const PEXCEPTION_POINTERS except_ptr) {
      const HANDLE dump_file = CreateFile(path.string().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
      if (dump_file == INVALID_HANDLE_VALUE) {
        LOG_ERROR("Failed to create dump file '{}'", path);
        return MakeFailure<ResultCode::kIO_ERROR>("Failed to create dump file '{}'", path);
      }

      MINIDUMP_EXCEPTION_INFORMATION except_info;
      except_info.ThreadId = GetCurrentThreadId();
      except_info.ExceptionPointers = except_ptr;
      except_info.ClientPointers = FALSE;

      if (!MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dump_file, MiniDumpIgnoreInaccessibleMemory, &except_info, nullptr, nullptr)) {
        LOG_ERROR("Failed to write minidump to '{}'", path);
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to write minidump to '{}'", path);
      }

      CloseHandle(dump_file);
      return {};
    }

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
      if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, addr, reinterpret_cast<void*>(addr), ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction))) {
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

    std::string GetRegisters(const PCONTEXT ctx) {
      std::stringstream res;
      res << "\nDumping registers:\n";
      res << "Rip: " << common::conversion::AddrToHex(ctx->Rip) << "\n";
      res << "Rax: " << common::conversion::AddrToHex(ctx->Rax) << "\n";
      res << "Rbx: " << common::conversion::AddrToHex(ctx->Rbx) << "\n";
      res << "Rcx: " << common::conversion::AddrToHex(ctx->Rcx) << "\n";
      res << "Rdx: " << common::conversion::AddrToHex(ctx->Rdx) << "\n";
      res << "Rsi: " << common::conversion::AddrToHex(ctx->Rsi) << "\n";
      res << "Rdi: " << common::conversion::AddrToHex(ctx->Rdi) << "\n";
      res << "RSp: " << common::conversion::AddrToHex(ctx->Rsp) << "\n";
      res << "Rbp: " << common::conversion::AddrToHex(ctx->Rbp) << "\n";
      res << "R8: " << common::conversion::AddrToHex(ctx->R8) << "\n";
      res << "R9: " << common::conversion::AddrToHex(ctx->R9) << "\n";
      res << "R10: " << common::conversion::AddrToHex(ctx->R10) << "\n";
      res << "R11: " << common::conversion::AddrToHex(ctx->R11) << "\n";
      res << "R12: " << common::conversion::AddrToHex(ctx->R12) << "\n";
      res << "R13: " << common::conversion::AddrToHex(ctx->R13) << "\n";
      res << "R14: " << common::conversion::AddrToHex(ctx->R14) << "\n";
      res << "R15: " << common::conversion::AddrToHex(ctx->R15) << "\n";

      return res.str();
    }
  }

  StatusOr<std::string> WriteExceptionReport(PEXCEPTION_RECORD except_rec, PCONTEXT ctx, std::size_t stacktrace_skip_count) {
    std::stringstream msg;
    std::string exception_name = ExceptionCodeToStr(except_rec->ExceptionCode);

    msg << "***** EXCEPTION RECEIVED *****\n";
    msg << "***** Exception name: " << exception_name << " *****\n";
    msg << "***** Exception code: " << common::conversion::AddrToHex(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception pid: " << GetCurrentProcessId() << " *****\n";

    auto except_mod_res = win32::memory::GetModuleNameFromAddress(GetCurrentProcessId(), ctx->Rip);
    if (except_mod_res.error())
      return except_mod_res.error().Forward();

    msg << "***** Exception module: " << except_mod_res.value() << " *****\n";
    msg << "***** Exception address: 0x" << except_rec->ExceptionAddress << " *****\n";
    msg << "***** Exception flags: " << except_rec->ExceptionFlags << " *****\n";
    msg << "***** Exception instruction: " << GetInstructionsStr(ctx->Rip).value_or("Failed to decompile exception area.") << " *****\n";


    msg << "\nLoaded Modules:\n";

    auto mod_res = win32::GetProcessModules(GetCurrentProcessId());
    if (mod_res.error())
      return mod_res.error().Forward();

    std::vector<MODULEENTRY32> modules = mod_res.value();
    for (auto&& mod : modules) {
      msg << mod.szExePath << " addr: " << common::conversion::AddrToHex(reinterpret_cast<std::uintptr_t>(mod.modBaseAddr)) << " size: " << common::conversion::AddrToHex(mod.modBaseSize);
      msg << '\n';
    }

    msg << "\n***** STACKDUMP *****\n";
    msg << GetRegisters(ctx);
    msg << '\n' << std::stacktrace::current(stacktrace_skip_count) << '\n';

    std::filesystem::path report_dir = util::vfs::GetExceptionReports() / fmt::format("report_{}", common::util::time::GetTimeStamp());
    if (!std::filesystem::create_directories(report_dir)) {
      LOG_ERROR("Failed to create exception report directory '{}'", report_dir);
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to create exception report directory '{}'", report_dir);
    }

    std::filesystem::path minidump_path = report_dir / "minidump.dmp";
    EXCEPTION_POINTERS except_ptr;
    except_ptr.ExceptionRecord = except_rec;
    except_ptr.ContextRecord = ctx;
    auto dump_res = WriteMiniDump(minidump_path, &except_ptr);
    if (dump_res.error()) {
      LOG_ERROR(dump_res);
      return dump_res.error().GetResultMessage();
    }

    std::filesystem::path report_file_path = report_dir / "report.log";
    std::ofstream report_file(report_file_path);
    if (!report_file.is_open()) {
      LOG_ERROR("Failed to open report file for writing '{}'", report_file_path);
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to open report file '{}'", report_file_path);
    }

    report_file << RemoveDoubleSpaces(msg.str());

    return fmt::format("{}: Wrote exception report to '{}'", exception_name, report_file_path);
  }
}
