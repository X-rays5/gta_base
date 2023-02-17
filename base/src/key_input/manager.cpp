//
// Created by X-ray on 02/16/23.
//

#include "manager.hpp"

namespace gta_base::key_input {
  Manager::Manager() {
    kINPUT_MGR = this;
  }

  Manager::~Manager() {
    kINPUT_MGR = nullptr;
  }

  bool Manager::IsControlPressed(std::uint32_t key_bind_id) {
    if (IsInputBlocked())
      return false;

    misc::ScopedSpinlock lock(key_binds_lock_);

    auto it = key_binds_.find(key_bind_id);
    if (it != key_binds_.end()) {
      bool down = true;
      for (auto&& keyboard_key : it->second.keyboard_key_bind) {
        if (!keyboard_.IsKeyDown(keyboard_key))
          down = false;
      }
      if (down)
        return true;

      down = true;
      for (auto&& controller_key : it->second.controller_key_bind) {
        if (!controller_.IsKeyDown(controller_key))
          down = false;
      }
      if (down)
        return true;
    }

    return false;
  }
  bool Manager::IsControlJustPressed(std::uint32_t key_bind_id) {
    if (IsInputBlocked())
      return false;

    misc::ScopedSpinlock lock(key_binds_lock_);

    auto it = key_binds_.find(key_bind_id);
    if (it != key_binds_.end()) {
      bool down = true;
      for (auto&& keyboard_key : it->second.keyboard_key_bind) {
        if (!keyboard_.KeyJustReleased(keyboard_key))
          down = false;
      }
      if (down)
        return true;

      down = true;
      for (auto&& controller_key : it->second.controller_key_bind) {
        if (!controller_.KeyJustReleased(controller_key))
          down = false;
      }
      if (down)
        return true;
    }

    return false;
  }

  bool Manager::AddKeybinding(std::uint32_t key_bind_id, std::initializer_list<Keyboard::INPUT> keyboard_bind, std::initializer_list<Controller::INPUT> controller_bind) {
    misc::ScopedSpinlock lock(key_binds_lock_);
    if (key_binds_.contains(key_bind_id)) {
      LOG_ERROR("Key bind with id {} already exists.", key_bind_id);
      return false;
    }

    KeyBinding binding = {
    keyboard_bind,
    controller_bind
    };
    key_binds_[key_bind_id] = std::move(binding);
  }

  void Manager::RemoveKeybinding(std::uint32_t key_bind_id) {
    key_binds_lock_.Lock();
    key_binds_.erase(key_bind_id);
    key_binds_lock_.Unlock();
  }
}
