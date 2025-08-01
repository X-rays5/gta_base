//
// Created by X-ray on 01/06/2025.
//

#include "../key_state.hpp"

namespace base::menu::util {
  KeyState::KeyState(const std::vector<std::uint32_t>& keys, const std::chrono::milliseconds cooldown_ms) : cooldown_ms_(cooldown_ms) {
    for (auto&& key : keys) {
      key_registry_[key] = KeyStateData{false, false, {}};
    }

    key_event_handler_id_ = kKEY_EVENT_WATCHER->AddKeyEventListener(this);
  }

  KeyState::~KeyState() {
    kKEY_EVENT_WATCHER->RemoveKeyEventListener(key_event_handler_id_);
  }

  bool KeyState::WasKeyPressed(const std::uint32_t key_code) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    if (const auto it = key_registry_.find(key_code); it != key_registry_.end()) {
      if (auto& [is_down, was_pressed, last_check_time] = it->second; was_pressed) {
        if (!is_down) {
          was_pressed = false;
        }

        if (const auto now = std::chrono::steady_clock::now(); now - last_check_time >= cooldown_ms_) {
          last_check_time = now;
          return true;
        }
      }
    }

    return false;
  }

  bool KeyState::IsKeyDown(const std::uint32_t key_code) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    if (const auto it = key_registry_.find(key_code); it != key_registry_.end()) {
      if (auto& [is_down, _, last_check_time] = it->second; is_down) {
        if (const auto now = std::chrono::steady_clock::now(); now - last_check_time >= cooldown_ms_) {
          last_check_time = now;
          return true;
        }
      }
    }

    return false;
  }

  void KeyState::KeyDown(const std::uint32_t vk_key, ModifierKey) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    if (const auto it = key_registry_.find(vk_key); it != key_registry_.end()) {
      if (auto& [is_down, was_pressed, last_check_time] = it->second; !is_down) {
        is_down = true;
        was_pressed = true;
      }
    }
  }

  void KeyState::KeyUp(const std::uint32_t vk_key, ModifierKey) {
    common::concurrency::ScopedSpinlock lock(spinlock_);
    if (const auto it = key_registry_.find(vk_key); it != key_registry_.end()) {
      if (auto& [is_down, _, last_check_time] = it->second; is_down) {
        is_down = false;
      }
    }
  }
}