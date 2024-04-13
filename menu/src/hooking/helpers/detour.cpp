//
// Created by X-ray on 08/08/22.
//

#include "detour.hpp"
#include <utility>

#define PTR_TO_ADDR(ptr) reinterpret_cast<std::uintptr_t>(ptr)

namespace base::hooking {
  DetourHook::DetourHook(std::string name, void* src, void* dst) : name_(std::move(name)) {
    LOG_DEBUG("Creating detour for {} at 0x{:x} to 0x{:x}", name_, PTR_TO_ADDR(src), PTR_TO_ADDR(dst));
    detour_ = std::make_unique<PLH::NatDetour>(reinterpret_cast<std::uintptr_t>(src), reinterpret_cast<std::uintptr_t>(dst), &og_);
  }

  DetourHook::DetourHook(std::string name, const std::string& module, const std::string& src, void* dst) : name_(std::move(name)) {
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

    LOG_DEBUG("Creating detour for {} at 0x{:x} to 0x{:x}", name_, PTR_TO_ADDR(src_addr), PTR_TO_ADDR(dst));
    detour_ = std::make_unique<PLH::NatDetour>(reinterpret_cast<std::uintptr_t>(src_addr), reinterpret_cast<std::uintptr_t>(dst), &og_);
  }

  DetourHook::~DetourHook() {
    if (detour_->isHooked())
      detour_->unHook();
  }

  std::string DetourHook::GetName() const {
    return name_;
  }

  void DetourHook::Enable() {
    if (detour_->isHooked()) {
      LOG_WARN("Tried to enable already enabled hook {}", name_);
      return;
    }

    if (detour_->hook()) {
      LOG_INFO("Hook for {} has been enabled.", name_);
    } else {
      LOG_CRITICAL("Failed to enabled hook for '{}': {}", name_, PLH::ErrorLog::singleton().pop().msg);
      globals::kRUNNING = false;
    }
  }

  void DetourHook::Disable() {
    if (!detour_->isHooked()) {
      LOG_WARN("Tried to disable already disabled hook {}", name_);
      return;
    }

    if (detour_->unHook()) {
      LOG_INFO("Hook for {} has been disabled.", name_);
    } else {
      LOG_CRITICAL("Failed to disable hook for '{}': {}", name_, PLH::ErrorLog::singleton().pop().msg);
    }
  }
}
