//
// Created by X-ray on 08/08/22.
//

#include <MinHook.hpp>
#include <magic_enum.hpp>
#include "vmt.hpp"
#include "../../memory/scanner/handle.hpp"

namespace base::hooking {
  VmtHook::VmtHook(LPVOID vtable) { vtable_ = *((LPVOID**)vtable); }

  void VmtHook::Hook(const std::string& name, std::size_t index, LPVOID detour) {
    if (auto it = hooks_.find(index); it != hooks_.end()) { LOG_WARN("Tried to create hook: {} at idx: {}. But a hook with the name: {} already exists", name, index, it->second.name); }

    LPVOID target = vtable_[index];
    FixHookAddress(name, target);

    LPVOID og;

    if (auto status = MH_CreateHook(target, detour, &og); status != MH_OK) { LOG_CRITICAL("Failed to create hook '{}' at 0x{:X} (error: {})", name, uintptr_t(target), magic_enum::enum_name(status)); }

    hooks_[index] = {name, target, detour, og};
  }

  void VmtHook::Unhook(std::size_t index) {
    if (auto it = hooks_.find(index); it == hooks_.end()) { LOG_WARN("Tried to remove hook at idx: {} which doesn't exist", index); } else {
      if (auto status = MH_RemoveHook(it->second.target); status != MH_OK) { LOG_CRITICAL("Failed to remove hook at idx: {} with the name: {}", index, it->second.name); }

      hooks_.erase(it);
    }
  }

  void VmtHook::Enable(std::size_t index) {
    if (auto it = hooks_.find(index); it == hooks_.end()) { LOG_WARN("Tried to enable hook at idx: {} which doesn't exist", index); } else {
      if (auto status = MH_EnableHook(it->second.target); status != MH_OK)
        LOG_CRITICAL("Failed to enable hook at idx: {} with the name: {}", index, it->second.name);
      else
        LOG_DEBUG("Enabled hook at idx: {} with the name: {} src -> {}, dst -> {}, og -> {}", index, it->second.name, it->second.target, it->second.detour, it->second.original);
    }
  }

  void VmtHook::EnableAll() { for (auto&& hook : hooks_) { Enable(hook.first); } }

  void VmtHook::Disable(std::size_t index) { if (auto it = hooks_.find(index); it == hooks_.end()) { LOG_WARN("Tried to disable hook at idx: {} which doesn't exist", index); } else { if (auto status = MH_DisableHook(it->second.target); status != MH_OK) { LOG_CRITICAL("Failed to disable hook at idx: {} with the name: {}", index, it->second.name); } } }

  void VmtHook::DisableAll() { for (auto&& hook : hooks_) { Disable(hook.first); } }

  void VmtHook::FixHookAddress(const std::string& name, LPVOID target) {
    __try {
      auto ptr = memory::scanner::Handle(target);
      while (ptr.as<std::uint8_t&>() == 0xE9) { ptr = ptr.add(1).rip(); }
      target = ptr.as<void*>();
    }
    __except (ExpHandler(GetExceptionInformation(), name)) { [&name]() { LOG_CRITICAL("Failed to fix hook address for '{}'", name); }(); }
  }

  DWORD VmtHook::ExpHandler(PEXCEPTION_POINTERS exp, const std::string& name) { return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH; }
}
