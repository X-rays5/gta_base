//
// Created by X-ray on 08/24/22.
//

#include "stack_trace.hpp"
#include <mutex>
#include <vector>
#include <sstream>
#include <filesystem>
#include <map>
#include <psapi.h>
#include <dbghelp.h>
#include <process.h>
#include <intrin.h>

#pragma comment(lib, "DbgHelp.lib")

namespace gta_base::logger::stacktrace {
  std::uint32_t RECURSIVE_CRASH_CHECK = 0;

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

  // Using the given context, fill in all the stack frames.
  // Which then later can be interpreted to human-readable text
  void captureStackTrace(CONTEXT* context, std::vector<uint64_t>& frame_pointers) {
    DWORD machine_type = 0;
    STACKFRAME64 frame = {}; // force zeroing
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    #if defined(_M_ARM64)
    frame.AddrPC.Offset = context->Pc;
        frame.AddrFrame.Offset = context->Fp;
        frame.AddrStack.Offset = context->Sp;
        machine_type = IMAGE_FILE_MACHINE_ARM64;
    #elif defined(_M_ARM)
    frame.AddrPC.Offset = context->Pc;
        frame.AddrFrame.Offset = context->R11;
        frame.AddrStack.Offset = context->Sp;
        machine_type = IMAGE_FILE_MACHINE_ARM;
    #elif defined(_M_X64)
    frame.AddrPC.Offset = context->Rip;
    frame.AddrFrame.Offset = context->Rbp;
    frame.AddrStack.Offset = context->Rsp;
    machine_type = IMAGE_FILE_MACHINE_AMD64;
    #else
    frame.AddrPC.Offset = context->Eip;
        frame.AddrPC.Offset = context->Ebp;
        frame.AddrPC.Offset = context->Esp;
        machine_type = IMAGE_FILE_MACHINE_I386;
    #endif
    for (size_t index = 0; index < frame_pointers.size(); ++index) {
      if (StackWalk64(machine_type,
                      GetCurrentProcess(),
                      GetCurrentThread(),
                      &frame,
                      context,
                      nullptr,
                      SymFunctionTableAccess64,
                      SymGetModuleBase64,
                      nullptr)) {
        frame_pointers[index] = frame.AddrPC.Offset;
      } else {
        break;
      }
    }
  }

  // Gets module name as a std::string from an address
  void getAllModuleInfos(std::map<std::string, std::pair<uint64_t, uint64_t>>* map) {
    HMODULE hMods[1024];
    HANDLE hProcess = GetCurrentProcess();
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        // Get the full path to the module's file.
        char modNameCharArray[MAX_PATH];
        if (GetModuleFileNameExA(hProcess, hMods[i], modNameCharArray, sizeof(modNameCharArray) / sizeof(char))) {
          std::string modName(modNameCharArray);
          MODULEINFO info;
          if (GetModuleInformation(hProcess, hMods[i], &info, sizeof(MODULEINFO))) {
            std::map<uint64_t, uint64_t> modInfo;
            map->emplace(modName, std::make_pair((uint64_t) info.lpBaseOfDll, (uint64_t) info.SizeOfImage));
          }
        }
      }
    }
  }

  // Gets module name and offset from address
  std::string getModuleNameFromAddress(uint64_t address) {
    std::map<std::string, std::pair<uint64_t, uint64_t>> map;
    getAllModuleInfos(&map);
    for (auto const& [key, val]: map) {
      if (address > val.first && address < (val.first + val.second)) {
        std::string retn = key + "+" + common::AddrToHex(address - val.first);
        return retn;
      }
    }
    return "";
  }

  // extract readable text from a given stack frame. All thanks to
  // using SymFromAddr and SymGetLineFromAddr64 with the stack pointer
  std::string getSymbolInformation(const size_t index, const std::vector<uint64_t>& frame_pointers) {
    auto addr = frame_pointers[index];
    std::string frame_dump = "stack dump [" + std::to_string(index) + "]\t";

    DWORD64 displacement64;
    DWORD displacement;
    char symbol_buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
    auto* symbol = reinterpret_cast<SYMBOL_INFO*>(symbol_buffer);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    std::string lineInformation;
    std::string callInformation;
    if (SymFromAddr(GetCurrentProcess(), addr, &displacement64, symbol)) {
      callInformation.append(" ").append(std::string(symbol->Name, symbol->NameLen));
      if (SymGetLineFromAddr64(GetCurrentProcess(), addr, &displacement, &line)) {
        lineInformation.append(" ").append(line.FileName).append(" L: ");
        lineInformation.append(std::to_string(line.LineNumber - 1));
      } else {
        std::string moduleName = getModuleNameFromAddress(addr);
        if (!moduleName.empty()) {
          lineInformation.append(" ").append(moduleName);
        }
      }
    } else {
      std::string moduleName = getModuleNameFromAddress(addr);
      if (!moduleName.empty()) {
        lineInformation.append(" ").append(moduleName);
      }
      lineInformation.append(" ").append(common::AddrToHex(addr));
    }
    frame_dump.append(lineInformation).append(callInformation);
    return frame_dump;
  }

  // Retrieves all the symbols for the stack frames, fills them within a text representation and returns it
  std::string convertFramesToText(std::vector<uint64_t>& frame_pointers) {
    std::string dump; // slightly more efficient than ostringstream
    const size_t kSize = frame_pointers.size();
    for (size_t index = 0; index < kSize && frame_pointers[index]; ++index) {
      dump += getSymbolInformation(index, frame_pointers);
      dump += "\n";
    }
    return dump;
  }

  std::string GetFileExceptionOccured(PEXCEPTION_RECORD except_rec) {
    auto location = reinterpret_cast<std::uintptr_t>(except_rec->ExceptionAddress);

    HMODULE Hmods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(GetCurrentProcess(), Hmods, sizeof(Hmods), &cbNeeded)) {
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        // Get the full path to the module's file.
        char modNameCharArray[MAX_PATH];
        if (GetModuleFileNameExA(GetCurrentProcess(), Hmods[i], modNameCharArray, sizeof(modNameCharArray) / sizeof(char))) {
          std::string modName(modNameCharArray);
          MODULEINFO info;
          if (GetModuleInformation(GetCurrentProcess(), Hmods[i], &info, sizeof(MODULEINFO))) {
            auto min = reinterpret_cast<std::uintptr_t>(info.lpBaseOfDll);
            auto max = min + info.SizeOfImage;
            if (location >= min && location < max) {
              return std::filesystem::path(modName).filename().string();
            }
          }
        }
      }
    }

    return "UNKNOWN";
  }

  std::string GetExceptionString(PEXCEPTION_POINTERS except) {
    return GetExceptionString(except->ExceptionRecord, except->ContextRecord);
  }

  std::string GetExceptionString(PEXCEPTION_RECORD except_rec, PCONTEXT ctx) {
    #ifndef NDEBUG
    //__debugbreak();
    #endif

    std::stringstream msg;
    msg << "\n***** EXCEPTION RECEIVED *****\n";
    msg << "\n***** Received fatal exception: " << ExceptionCodeToStr(except_rec->ExceptionCode) << " pid: " << _getpid() << " module: " << GetFileExceptionOccured(except_rec) << " *****\n";
    msg << "***** Exception code: " << common::AddrToHex(except_rec->ExceptionCode) << " *****\n";
    msg << "***** Exception address: 0x" << except_rec->ExceptionAddress << " *****\n";
    msg << "***** Exception instruction: " << common::GetInstructionStr((std::uintptr_t) except_rec->ExceptionAddress) << " *****\n";
    msg << "***** Exception flags: " << except_rec->ExceptionFlags << " *****\n";

    msg << "\n***** STACKDUMP *****\n";

    if (RECURSIVE_CRASH_CHECK >= 2) {
      msg << "\n\n\n***** Recursive crash detected aborting stackdump traversal. *****\n\n\n";
      return msg.str();
    }
    RECURSIVE_CRASH_CHECK += 1;

    static std::mutex mtx;
    std::lock_guard lock(mtx);
    {

      if (!SymInitialize(GetCurrentProcess(), nullptr, true)) {
        return "GetExceptionString() error: Failed to init SymInitialize() for retrieving symbols in stack";
      }

      std::shared_ptr<void> RaiiSymCleaner(nullptr, [&](void*) {
        SymCleanup(GetCurrentProcess());
      });

      msg << "\nLoaded Modules:\n";
      std::map<std::string, std::pair<uint64_t, uint64_t>> map;
      getAllModuleInfos(&map);
      for (auto const& [key, val]: map) {
        msg << key + " Base Address: " + common::AddrToHex(val.first) + " Size: " + common::AddrToHex(val.second);
        msg << "\n";
      }

      msg << GetRegisters(ctx);

      const size_t kmax_frame_dump_size = 64;
      std::vector<uint64_t> frame_pointers(kmax_frame_dump_size);
      captureStackTrace(ctx, frame_pointers);
      msg << "\n" << convertFramesToText(frame_pointers) << "\n";

      return RemoveDoubleSpaces(msg.str());
    }

    return RemoveDoubleSpaces(msg.str());
  }
}