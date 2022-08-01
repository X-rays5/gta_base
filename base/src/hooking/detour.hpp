//
// Created by X-ray on 08/01/22.
//

#pragma once
#ifndef GTA_BASE_DETOUR_HPP
#define GTA_BASE_DETOUR_HPP
#include <string>
#include <MinHook.h>
#include "../memory/sigscanner.hpp"

namespace gta_base {
  namespace hooking {
    class DetourHook {
    public:
      explicit DetourHook(std::string name, void *target, void *detour) : name_(std::move(name)), target_(target), detour_(detour) {
        FixHookAddress();

        if (auto status = MH_CreateHook(target_, detour_, &original_); status == MH_OK) {
          LOG_INFO << "Created hook '" << name_ << "'.";
        } else {
          LOG_FATAL << fmt::format("Failed to create hook '{}' at 0x{:X} (error: {})", name_, uintptr_t(target_), MH_StatusToString(status));
        }
      }

      ~DetourHook() noexcept {
        if (target_) {
          MH_RemoveHook(target_);
        }

        LOG_INFO << "Removed hook '" << name_ << "'.";
      }

      DetourHook(DetourHook&& that) = delete;
      DetourHook& operator=(DetourHook&& that) = delete;
      DetourHook(DetourHook const&) = delete;
      DetourHook& operator=(DetourHook const&) = delete;

      inline std::string GetName() const {
        return name_;
      }

      void Enable() {
        if (auto status = MH_EnableHook(target_); status == MH_OK) {
          LOG_INFO << "Enabled hook '" << name_ << "'.";
          #ifndef NDEBUG
            LOG_DEBUG << fmt::format("[{}] hooked src -> {}, dst -> {}, og -> {}", name_, (void*)target_, (void*)detour_, (void*)original_);
          #endif
        }
        else {
          LOG_FATAL << fmt::format("Failed to enable hook 0x{:X} ({})", uintptr_t(target_), MH_StatusToString(status));
        }
      }

      void Disable() {
        if (auto status = MH_DisableHook(target_); status == MH_OK) {
          LOG_INFO << "Disabled hook '" << name_ << "'.";
        }
        else {
          LOG_WARNING << "Failed to disable hook '" << name_ << "'.";
        }
      }

      template <typename T>
      T GetOriginal() {
        return static_cast<T>(original_);
      }
    private:
      std::string name_;
      void *target_;
      void *detour_;
      void *original_{};

    private:
      void FixHookAddress() {
        __try {
          auto ptr = memory::MemoryHandle(target_);
          while (ptr.As<std::uint8_t&>() == 0xE9)
          ptr = ptr.Add(1).Rip();
          target_ = ptr.As<void*>();
        } __except (ExpHandler(GetExceptionInformation(), name_)) {
          [this]() {
            LOG_FATAL << fmt::format("Failed to fix hook address for '{}'", name_);
          }();
        }
      }

      static DWORD ExpHandler(PEXCEPTION_POINTERS exp, std::string const& name) {
        return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
               ? EXCEPTION_EXECUTE_HANDLER
               : EXCEPTION_CONTINUE_SEARCH;
      }
    };
  }
}
#endif //GTA_BASE_DETOUR_HPP
