//
// Created by X-ray on 02/16/23.
//

#include "manager.hpp"

namespace gta_base::key_input {
  namespace {
    FORCE_INLINE bool IsKeyboardPressed(Keyboard* keyboard, const std::vector<Keyboard::INPUT>& keyboard_input) {
      for (auto&& key : keyboard_input) {
        if (!keyboard->IsKeyDown(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsKeyboardJustReleased(Keyboard* keyboard, const std::vector<Keyboard::INPUT>& keyboard_input) {
      for (auto&& key : keyboard_input) {
        if (!keyboard->KeyJustReleased(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsControllerPressed(Controller* controller, const std::vector<Controller::INPUT>& controller_input) {
      for (auto&& key : controller_input) {
        if (!controller->IsKeyDown(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsControllerJustReleased(Controller* controller, const std::vector<Controller::INPUT>& controller_input) {
      for (auto&& key : controller_input) {
        if (!controller->KeyJustReleased(key))
          return false;
      }

      return true;
    }
  }

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
      if (IsKeyboardPressed(&keyboard_, it->second.keyboard_key_bind))
        return true;
      else if (IsControllerPressed(&controller_, it->second.controller_key_bind))
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
      if (IsKeyboardJustReleased(&keyboard_, it->second.keyboard_key_bind))
        return true;
      else if (IsControllerJustReleased(&controller_, it->second.controller_key_bind))
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
    controller_bind,
    };
    key_binds_[key_bind_id] = std::move(binding);

    return true;
  }

  void Manager::RemoveKeybinding(std::uint32_t key_bind_id) {
    key_binds_lock_.Lock();
    key_binds_.erase(key_bind_id);
    key_binds_lock_.Unlock();
  }
}
