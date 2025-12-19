//
// Created by xray on 06/09/2023.
//

#include "vectored_handler.hpp"
#include <Zydis/Zydis.h>
#include "exception_report.hpp"
#include "util.hpp"
#include "../logger.hpp"
#include "../../win32/memory.hpp"

#define VECTORED_HANDLER_ATTEMPT_RECOVERY

namespace base::common::logging::exception {
  namespace {
    PVOID cur_handler = nullptr;

    // Thread-local variables to track recovery attempts
    thread_local std::uint32_t recovery_attempts = 0;
    thread_local std::chrono::steady_clock::time_point last_exception_time{};

    // Configuration constants
    constexpr std::uint32_t MAX_RECOVERY_ATTEMPTS = 100;
    constexpr std::chrono::milliseconds RECOVERY_RESET_TIMEOUT{5000};

    std::optional<ZydisDecodedInstruction> GetInstructionAtAddr(const std::uintptr_t addr) {
      ZydisDecoder decoder;
#ifdef _M_AMD64
      if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)))
#else
      if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_STACK_WIDTH_32)))
#endif
        return std::nullopt;

      ZydisDecodedInstruction instruction{};
      ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
      if (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, reinterpret_cast<void*>(addr), 15, &instruction, operands))) {
        return instruction;
      }

      return std::nullopt;
    }

    bool ShouldAttemptRecovery(std::uintptr_t exception_address) {
      const auto now = std::chrono::steady_clock::now();

      if (now - last_exception_time > RECOVERY_RESET_TIMEOUT) {
        recovery_attempts = 0;
        last_exception_time = now;
      }

      if (recovery_attempts >= MAX_RECOVERY_ATTEMPTS) {
        LOG_ERROR("Maximum recovery attempts ({}) reached for address {:X}, giving up",  MAX_RECOVERY_ATTEMPTS, exception_address);
        return false;
      }

      return true;
    }

    bool SkipToNextInstruction(const PCONTEXT ctx) {
      const auto exception_address = ctx->Rip;

      if (!ShouldAttemptRecovery(exception_address)) {
        return false;
      }

      const auto instruction_opt = GetInstructionAtAddr(exception_address);
      if (!instruction_opt.has_value()) {
        LOG_ERROR("Failed to decode instruction at {:X}", exception_address);
        recovery_attempts++; // Count failed decode as an attempt
        return false;
      }

      const auto& instruction = instruction_opt.value();
      auto next_instruction = exception_address + instruction.length;

      recovery_attempts++;

      LOG_WARN("Trying to recover from exception (attempt {}/{}): {:X} -> {:X}",  recovery_attempts, MAX_RECOVERY_ATTEMPTS, exception_address, next_instruction);

      ctx->Rip = next_instruction;
      return true;
    }

    void MSVCException(const PEXCEPTION_POINTERS except) {
      const auto exception_address = except->ContextRecord->Rip;

      // Only log exceptions from our own module
      if (!win32::memory::IsAddressInCurrentModule(exception_address)) {
        return;
      }

      const std::uint32_t pid = GetCurrentProcessId();

      auto mod_name_res = win32::memory::GetModuleNameFromAddress(pid, exception_address);
      std::string mod_name;
      if (mod_name_res.has_value()) {
        mod_name = mod_name_res.value();
      }

      auto offset_res = win32::memory::GetModuleOffsetFromAddress(pid, exception_address);
      std::uintptr_t offset = 0;
      if (offset_res.has_value()) {
        offset = offset_res.value();
      }

      const auto* exception = std::bit_cast<std::exception*>(except->ExceptionRecord->ExceptionInformation[1]);
      if (exception && exception->what()) {
        LOG_ERROR("{}+{}: {}", mod_name, offset, exception->what());
      } else {
        LOG_ERROR("{}+{}: cpp exception thrown", mod_name, offset);
      }
    }

    LONG VectoredExceptionHandler(const PEXCEPTION_POINTERS except) {
      auto err_code = except->ExceptionRecord->ExceptionCode;

      if (err_code == EXCEPTION_BREAKPOINT || err_code == EXCEPTION_SINGLE_STEP) {
        return EXCEPTION_CONTINUE_SEARCH;
      }

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

#ifdef VECTORED_HANDLER_ATTEMPT_RECOVERY
      // We will now attempt to recover by moving the RIP to the next instruction (Yes I'm serious)
      if (SkipToNextInstruction(except->ContextRecord)) {
        return EXCEPTION_CONTINUE_EXECUTION;
      }
#endif

      if (auto stacktrace_res = WriteExceptionReport(except, 7); stacktrace_res.has_value()) {
        LOG_CRITICAL(stacktrace_res.value());
      }

#ifndef VECTORED_HANDLER_UNIT_TEST
      spdlog::default_logger_raw()->flush();

      Manager::Shutdown();
#endif

      return EXCEPTION_CONTINUE_SEARCH;
    }
  }

  void EnableHandler() {
    cur_handler = AddVectoredExceptionHandler(false, VectoredExceptionHandler);
  }

  void DisableHandler() {
    RemoveVectoredExceptionHandler(cur_handler);
    cur_handler = nullptr;
  }
}
