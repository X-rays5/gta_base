//
// Created by X-ray on 01/06/2025.
//

#include "key_event_listener.hpp"

namespace base::menu::util {
  namespace {
    KeyEventListener::ModifierKey DetermineModifierKey() {
      if (GetKeyState(VK_SHIFT) & 0x8000) {
        return KeyEventListener::ModifierKey::kSHIFT;
      }

      if (GetKeyState(VK_CONTROL) & 0x8000) {
        return KeyEventListener::ModifierKey::kCTRL;
      }

      return KeyEventListener::ModifierKey::kNONE;
    }
  }

  KeyEventWatcher::KeyEventWatcher() {
    wndproc_handler_id_ = hooking::kWNDPROC->AddWndProcHandler(this);

    kKEY_EVENT_WATCHER = this;
  }

  KeyEventWatcher::~KeyEventWatcher() {
    kKEY_EVENT_WATCHER = nullptr;

    hooking::kWNDPROC->RemoveWndProcHandler(wndproc_handler_id_);
  }

  void KeyEventWatcher::OnWndProc(HWND, const UINT msg, const WPARAM wparam, LPARAM) {
    if (msg == WM_KEYDOWN) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      for (auto& handler : event_listeners_ | std::views::values) {
        if (handler) {
          handler->KeyDown(static_cast<std::uint32_t>(wparam), DetermineModifierKey());
        }
      }
    } else if (msg == WM_KEYUP) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      for (auto& handler : event_listeners_ | std::views::values) {
        if (handler) {
          handler->KeyUp(static_cast<std::uint32_t>(wparam), DetermineModifierKey());
        }
      }
    }
  }

  std::size_t KeyEventWatcher::AddKeyEventListener(KeyEventListener* listener) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    const std::size_t id = next_event_handler_id_.fetch_add(1);
    event_listeners_[id] = listener;
    LOG_DEBUG("Added KeyEventListener with id: {}", id);
    return id;
  }

  void KeyEventWatcher::RemoveKeyEventListener(const std::size_t id) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    if (const auto it = event_listeners_.find(id); it != event_listeners_.end()) {
      event_listeners_.erase(it); // Just remove the reference, don't delete
      LOG_DEBUG("Removed KeyEventListener with id: {}", id);
    } else {
      LOG_WARN("Attempted to remove non-existing KeyEventListener with id: {}", id);
    }
  }
}