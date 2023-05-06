//
// Created by X-ray on 02/16/23.
//

#include "controller.hpp"
#include <magic_enum.hpp>

namespace gta_base::key_input {
  Controller::Controller() {
    controller_state_update_thread_ = std::thread([this] { UpdateState(this); });
  }

  Controller::~Controller() {
    running_ = false;
    controller_state_update_thread_.join();
  }

  bool Controller::IsKeyDown(Controller::INPUT key) {
    misc::ScopedSpinlock lock(lock_);
    auto it = key_state_.find(key);

    if (it != key_state_.end()) {
      return it->second.down;
    }
    return false;
  }

  bool Controller::KeyJustReleased(Controller::INPUT key, std::uint64_t time_released_ms) {
    misc::ScopedSpinlock lock(lock_);
    auto it = key_state_.find(key);

    if (it != key_state_.end()) {
      return !it->second.down && common::GetEpoch() - it->second.released_at <= time_released_ms;
    }

    return false;
  }

  void Controller::UpdateState(Controller* that) {
    while (that->running_) {
      XINPUT_KEYSTROKE keystroke;
      ZeroMemory(&keystroke, sizeof(XINPUT_KEYSTROKE));

      //auto res_code = XInputGetKeystroke(XUSER_INDEX_ANY, NULL, &keystroke);
      int res_code = 1;
      if (res_code == ERROR_SUCCESS) {
        if (magic_enum::enum_contains<INPUT>(keystroke.VirtualKey)) {
          misc::ScopedSpinlock scoped_spinlock(that->lock_);

          if (keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN)
            that->key_state_[static_cast<INPUT>(keystroke.VirtualKey)] = {.down = true};
          else if (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP)
            that->key_state_[static_cast<INPUT>(keystroke.VirtualKey)] = {false, common::GetEpoch()};
        }
      }

      std::this_thread::yield();
    }
  }
}