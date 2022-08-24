//
// Created by X-ray on 08/08/22.
//

#include <MinHook.h>
#include "vmt.hpp"
#include "../memory/sigscanner.hpp"

namespace gta_base {
  namespace hooking {
    VmtHook::VmtHook(LPVOID vtable) {
      vtable_ = *((LPVOID**)vtable);
    }

    void VmtHook::Hook(const std::string& name, std::size_t index, LPVOID detour) {
      if (auto it = hooks_.find(index); it != hooks_.end()) {
        LOG_WARN("Tried to create hook: {} at idx: {}. But a hook with the name: {} already exists", name, index, it->second.name);
      }

      LPVOID target = vtable_[index];
      FixHookAddress(name, target);

      LPVOID og;

      if (auto status = MH_CreateHook(target, detour, &og); status == MH_OK) {
        LOG_INFO("Created hook '{}'.", name);
      } else {
        LOG_FATAL("Failed to create hook '{}' at 0x{:X} (error: {})", name, uintptr_t(target), MH_StatusToString(status));
      }

      hooks_[index] = {name, target, detour, og};
    }

    void VmtHook::Unhook(std::size_t index) {
      if (auto it = hooks_.find(index); it == hooks_.end()) {
        LOG_WARN("Tried to remove hook at idx: {} which doesn't exist", index);
      } else {
        if (auto status = MH_RemoveHook(it->second.target); status == MH_OK) {
          LOG_INFO("Removed hook at idx: {} with the name: {}", index, it->second.name);
        } else {
          LOG_FATAL("Failed to remove hook at idx: {} with the name: {}", index, it->second.name);
        }

        hooks_.erase(it);
      }
    }

    void VmtHook::Enable(std::size_t index) {
      if (auto it = hooks_.find(index); it == hooks_.end()) {
        LOG_WARN("Tried to enable hook at idx: {} which doesn't exist", index);
      } else {
        if (auto status = MH_EnableHook(it->second.target); status == MH_OK) {
          LOG_INFO("Enabled hook at idx: {} with the name: {}", index, it->second.name);
        } else {
          LOG_FATAL("Failed to enable hook at idx: {} with the name: {}", index, it->second.name);
        }
      }
    }

    void VmtHook::Disable(std::size_t index) {
      if (auto it = hooks_.find(index); it == hooks_.end()) {
        LOG_WARN("Tried to disable hook at idx: {} which doesn't exist", index);
      } else {
        if (auto status = MH_DisableHook(it->second.target); status == MH_OK) {
          LOG_INFO("Disabled hook at idx: {} with the name: {}", index, it->second.name);
        } else {
          LOG_FATAL("Failed to disable hook at idx: {} with the name: {}", index, it->second.name);
        }
      }
    }

    void VmtHook::FixHookAddress(const std::string& name, LPVOID target) {
      __try {
        auto ptr = memory::MemoryHandle(target);
        while (ptr.As<std::uint8_t&>() == 0xE9)
        ptr = ptr.Add(1).Rip();
        target = ptr.As<void*>();
      } __except (ExpHandler(GetExceptionInformation(), name)) {
        [&name]() {
          LOG_FATAL("Failed to fix hook address for '{}'", name);
        }();
      }
    }

    DWORD VmtHook::ExpHandler(PEXCEPTION_POINTERS exp, const std::string& name) {
      return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
    }
  }
}