//
// Created by X-ray on 08/08/22.
//

#include "detour.hpp"
#include <MinHook.h>
#include "../../logger/logger.hpp"
#include "../../memory/scanner/handle.hpp"

namespace gta_base {
  namespace hooking {
    DetourHook::DetourHook(std::string name, void* target, void* detour) : name_(std::move(name)), target_(target), detour_(detour) {
      FixHookAddress();

      if (auto status = MH_CreateHook(target_, detour_, &original_); status == MH_OK) {
        LOG_INFO("Created hook '{}'.", name_);
      } else {
        LOG_FATAL("Failed to create hook '{}' at 0x{:X} (error: {})", name_, uintptr_t(target_), MH_StatusToString(status));
      }
    }

    DetourHook::~DetourHook() noexcept {
      if (target_) {
        MH_RemoveHook(target_);
      }

      LOG_INFO("Removed hook '{}'.", name_);
    }

    std::string DetourHook::GetName() const {
      return name_;
    }

    void DetourHook::Enable() {
      if (auto status = MH_EnableHook(target_); status == MH_OK) {
        LOG_INFO("Enabled hook '{}'.", name_);
        LOG_DEBUG("[{}] hooked src -> {}, dst -> {}, og -> {}", name_, (void*)target_, (void*)detour_, (void*)original_);
      }
      else {
        LOG_FATAL("Failed to enable hook 0x{:X} ({})", uintptr_t(target_), MH_StatusToString(status));
      }
    }

    void DetourHook::Disable() {
      if (auto status = MH_DisableHook(target_); status == MH_OK) {
        LOG_INFO("Disabled hook '{}'.", name_);
      }
      else {
        LOG_WARN("Failed to disable hook '{}'.", name_);
      }
    }

    void DetourHook::FixHookAddress() {
      __try {
        auto ptr = memory::scanner::Handle(target_);
        while (ptr.as<std::uint8_t&>() == 0xE9)
        ptr = ptr.add(1).rip();
        target_ = ptr.as<void*>();
      } __except (ExpHandler(GetExceptionInformation(), name_)) {
        [this]() {
          LOG_FATAL("Failed to fix hook address for '{}'", name_);
        }();
      }
    }

    DWORD DetourHook::ExpHandler(PEXCEPTION_POINTERS exp, const std::string& name) {
      return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
    }
  }
}