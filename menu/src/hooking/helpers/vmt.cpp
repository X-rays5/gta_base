//
// Created by X-ray on 08/08/22.
//

#include "vmt.hpp"
#include <ranges>
#include <polyhook2/ErrorLog.hpp>

namespace base::menu::hooking {
  VmtHook::VmtHook(const std::string& vtable_name, LPVOID vtable, std::initializer_list<IDXHook> hooks) : vtable_name_(vtable_name), vtable_(vtable) {
    for (auto&& [idx, dst] : hooks) {
      vtable_redirect_.insert({idx, reinterpret_cast<std::uintptr_t>(dst)});
    }
  }

  VmtHook::~VmtHook() {
    if (vtable_hook_ && vtable_hook_->isHooked())
      vtable_hook_->unHook();
  }

  void VmtHook::EnableAll() {
    if (vtable_hook_ && vtable_hook_->isHooked()) {
      LOG_DEBUG("Tried to enable vmt hook '{}' but it's already enabled", vtable_name_);
      return;
    }

    vtable_hook_ = std::make_unique<PLH::VFuncSwapHook>(reinterpret_cast<std::uintptr_t>(vtable_), vtable_redirect_, &vtable_og_);
    if (vtable_hook_->hook()) {
      LOG_INFO("Vmt hook '{}' has been enabled", vtable_name_);
    } else {
      LOG_CRITICAL("Failed to enable vmt hook '{}': {}", vtable_name_, PLH::ErrorLog::singleton().pop().msg);
      globals::kRUNNING = false;
    }
  }

  void VmtHook::DisableAll() {
    if (vtable_hook_ && !vtable_hook_->isHooked()) {
      LOG_DEBUG("Tried to disable vmt hook '{}' but it's already disabled", vtable_name_);
      return;
    }

    if (vtable_hook_->unHook()) {
      LOG_INFO("Vmt hook '{}' has been disabled", vtable_name_);
    } else {
      LOG_CRITICAL("Failed to disable vmt hook '{}': {}", vtable_name_, PLH::ErrorLog::singleton().pop().msg);
    }
  }
}
