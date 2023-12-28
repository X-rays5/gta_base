//
// Created by X-ray on 08/08/22.
//

#include "detour.hpp"
#include <utility>
#include <Zydis/Zydis.h>

#define PTR_TO_ADDR(ptr) reinterpret_cast<std::uintptr_t>(ptr)

namespace base::hooking {
  namespace {
    int DisasmHandler(void* src, int* reloc_op_offset) {
      ZydisDecoder decoder;
#ifdef _M_AMD64
      if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)))
#else
      if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_STACK_WIDTH_32)))
#endif
        return NULL;

      ZydisDecodedInstruction instruction;
      ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
      if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, src, 15, &instruction, operands))) {
        LOG_FATAL("Failed to disasm op.");
      }

      // Check for the operand type that indicates a relative address, e.g., CALL or JMP.
      for (unsigned int i = 0; i < instruction.operand_count; ++i) {
        if (operands[i].type == ZYDIS_OPERAND_TYPE_IMMEDIATE && operands[i].imm.is_relative) {
          // Calculate the offset of the relative address within the instruction.
          uintptr_t immediate_address = (uintptr_t)src + instruction.raw.imm[i].offset;
          auto instruction_address = reinterpret_cast<std::uintptr_t>(src);
          *reloc_op_offset = static_cast<std::int32_t>(immediate_address - instruction_address);
          return instruction.length;
        }
      }

      return instruction.length;
    }
  }

  DetourHook::DetourHook(std::string name, void* src, void* dst) : name_(std::move(name)) {
    detour_ = subhook_new(src, dst, static_cast<subhook_flags_t>(SUBHOOK_64BIT_OFFSET | SUBHOOK_TRAMPOLINE));
    subhook_set_disasm_handler(DisasmHandler);
  }

  DetourHook::DetourHook(std::string name, std::string module, std::string src, void* dst) : name_(std::move(name)) {
    const HMODULE target_dll = GetModuleHandleA(module.c_str());
    if (!target_dll) {
      LOG_CRITICAL("Failed to find target module {} for {} hook", module, name_);
      return;
    }

    FARPROC src_addr = GetProcAddress(target_dll, src.c_str());
    if (!src_addr) {
      LOG_ERROR("Failed to get address of '{}' in '{}'", src, module);
      return;
    }

    detour_ = subhook_new(src_addr, dst, static_cast<subhook_flags_t>(SUBHOOK_64BIT_OFFSET | SUBHOOK_TRAMPOLINE));
  }

  DetourHook::~DetourHook() {
    subhook_free(detour_);
  }

  std::string DetourHook::GetName() const {
    return name_;
  }

  void DetourHook::Enable() {
    if (const auto status = absl::ErrnoToStatus(subhook_install(detour_), ""); status.ok()) {
      LOG_INFO("Hook for {} has been enabled.", name_);
    } else {
      LOG_CRITICAL("Failed to enabled hook for {} error {}.", name_, absl::StatusCodeToString(status.code()));
    }
  }

  void DetourHook::Disable() {
    if (const auto status = absl::ErrnoToStatus(subhook_remove(detour_), ""); status.ok()) {
      LOG_INFO("Hook for {} has been enabled.", name_);
    } else {
      LOG_CRITICAL("Failed to disable hook for {} error {}.", name_, absl::StatusCodeToString(status.code()));
    }
  }
}
