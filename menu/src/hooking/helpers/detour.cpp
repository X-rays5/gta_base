//
// Created by X-ray on 08/08/22.
//

#include "detour.hpp"
#include <utility>
#include <MinHook.hpp>
#include <magic_enum.hpp>

#include "../../memory/scanner/handle.hpp"

namespace base::hooking {
  DetourHook::DetourHook(std::string name, void* target, void* detour)
    :
    name_(std::move(name)), target_(target), detour_(detour) {
    FixHookAddress();

    if (auto status = MH_CreateHook(target_, detour_, &original_); status != MH_OK) { LOG_ERROR("Failed to create hook '{}' at 0x{:X} (error: {})", name_, uintptr_t(target_), magic_enum::enum_name(status)); }
  }

  DetourHook::DetourHook(std::string name, std::string module_name, std::string func_name, void* detour)
    :
    name_(std::move(name)), detour_(detour) {
    auto target_dll = GetModuleHandleA(module_name.c_str());
    if (!target_dll) {
      LOG_ERROR("Failed to get module handle for '{}'", module_name);
      return;
    }

    target_ = GetProcAddress(target_dll, func_name.c_str());
    if (!target_) {
      LOG_ERROR("Failed to get address of '{}' in '{}'", func_name, module_name);
      return;
    }

    if (auto status = MH_CreateHook(target_, detour_, &original_); status != MH_OK) { LOG_ERROR("Failed to create hook '{}' at 0x{:X} (error: {})", name_, uintptr_t(target_), magic_enum::enum_name(status)); }
  }

  DetourHook::~DetourHook() noexcept { if (target_) { MH_RemoveHook(target_); } }

  std::string DetourHook::GetName() const { return name_; }

  void DetourHook::Enable() { if (auto status = MH_EnableHook(target_); status == MH_OK) { LOG_DEBUG("[{}] hooked src -> {}, dst -> {}, og -> {}", name_, (void*) target_, (void*) detour_, (void*) original_); } else { LOG_ERROR("Failed to enable hook 0x{:X} ({})", uintptr_t(target_), magic_enum::enum_name(status)); } }

  void DetourHook::Disable() { if (auto status = MH_DisableHook(target_); status != MH_OK) { LOG_ERROR("Failed to disable hook '{}'.", name_); } }

  void DetourHook::FixHookAddress() {
    __try {
      auto ptr = memory::scanner::Handle(target_);
      while (ptr.as<std::uint8_t&>() == 0xE9) { ptr = ptr.add(1).rip(); }
      target_ = ptr.as<void*>();
    }
    __except (ExpHandler(GetExceptionInformation(), name_)) { [this]() { LOG_CRITICAL("Failed to fix hook address for '{}'", name_); }(); }
  }

  DWORD DetourHook::ExpHandler(PEXCEPTION_POINTERS exp, [[maybe_unused]] const std::string& name) { return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH; }
}
