//
// Created by X-ray on 08/08/22.
//

#include "detour.hpp"
#include <utility>
#include <Zydis/Zydis.h>
#include "../../memory/scanner/handle.hpp"

#define PTR_TO_ADDR(ptr) reinterpret_cast<std::uintptr_t>(ptr)

namespace base::hooking {
  namespace {
    int DisasmHandler(void* src, int* reloc_op_offset) {
      ZydisDisassembledInstruction instruction;
      if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, PTR_TO_ADDR(src), src, 15, &instruction))) {
        LOG_FATAL("Failed to disasm op.");
      }

      auto ptr = memory::scanner::Handle(src);
      while (ptr.as<std::uint8_t&>() == 0xE9) {
        ptr = ptr.add(1).rip();
      }
      *reloc_op_offset = static_cast<std::int32_t>(ptr.as<std::uintptr_t>() - PTR_TO_ADDR(src));

      return instruction.info.length;
    }
  }

  DetourHook::DetourHook(std::string name, void* src, void* dst) : name_(std::move(name)) {
    subhook_set_disasm_handler(DisasmHandler);
    detour_ = std::make_unique<subhook::Hook>(src, dst, static_cast<subhook::HookFlags>(SUBHOOK_64BIT_OFFSET | SUBHOOK_TRAMPOLINE));
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

    detour_ = std::make_unique<subhook::Hook>(src_addr, dst, static_cast<subhook::HookFlags>(SUBHOOK_64BIT_OFFSET | SUBHOOK_TRAMPOLINE));
  }

  DetourHook::~DetourHook() {
    if (detour_->IsInstalled())
      detour_->Remove();
  }

  std::string DetourHook::GetName() const {
    return name_;
  }

  void DetourHook::Enable() {
    if (detour_->IsInstalled()) {
      LOG_WARN("Tried to enable already enabled hook {}", name_);
      return;
    }

    if (detour_->Install()) {
      LOG_INFO("Hook for {} has been enabled.", name_);
    } else {
      LOG_CRITICAL("Failed to enabled hook for {}", name_);
    }
  }

  void DetourHook::Disable() {
    if (!detour_->IsInstalled()) {
      LOG_WARN("Tried to disable already disabled hook {}", name_);
      return;
    }

    if (detour_->Remove()) {
      LOG_INFO("Hook for {} has been disabled.", name_);
    } else {
      LOG_CRITICAL("Failed to disable hook for {}.", name_);
    }
  }
}
