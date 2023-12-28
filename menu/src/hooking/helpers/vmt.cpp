//
// Created by X-ray on 08/08/22.
//

#include "vmt.hpp"
#include <ranges>

namespace base::hooking {
  VmtHook::VmtHook(LPVOID vtable) : vtable_(*((LPVOID**)vtable)) {}

  void VmtHook::Hook(const std::string& name, std::size_t idx, LPVOID dst) {
    if (auto it = hooks_.find(idx); it != hooks_.end()) {
      LOG_ERROR("Tried to create hook: {} at idx: {}. But a hook with the name: {} already exists", name, idx, it->second->GetName());
      return;
    }

    LPVOID target = vtable_[idx];

    hooks_[idx] = std::make_unique<DetourHook>(name, target, dst);
  }

  void VmtHook::Enable(const std::size_t idx) {
    const auto it = hooks_.find(idx);

    if (it != hooks_.end()) {
      it->second->Enable();
    }
  }

  void VmtHook::EnableAll() {
    for (auto&& hook : std::views::keys(hooks_)) {
      Enable(hook);
    }
  }

  void VmtHook::Disable(const std::size_t idx) {
    const auto it = hooks_.find(idx);

    if (it != hooks_.end()) {
      it->second->Disable();
    }
  }

  void VmtHook::DisableAll() {
    for (auto&& hook : std::views::keys(hooks_)) {
      Disable(hook);
    }
  }
}
