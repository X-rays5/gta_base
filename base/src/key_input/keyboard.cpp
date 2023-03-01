//
// Created by X-ray on 02/16/23.
//

#include "keyboard.hpp"
#include "../hooking/wndproc.hpp"

namespace gta_base::key_input {
  Keyboard::Keyboard() {
    hooking::kWND_PROC_HOOK->AddHandler(WndProc);
    kKEYBOARD = this;
  }

  Keyboard::~Keyboard() {
    kKEYBOARD = nullptr;
  }

  bool Keyboard::IsKeyDown(Keyboard::INPUT key) {
    misc::ScopedSpinlock lock(lock_);
    auto it = key_state_.find(key);

    if (it != key_state_.end()) {
      return it->second.down;
    }
    return false;
  }

  bool Keyboard::KeyJustReleased(Keyboard::INPUT key, std::uint64_t time_released_ms) {
    misc::ScopedSpinlock lock(lock_);
    auto it = key_state_.find(key);

    if (it != key_state_.end()) {
      return !it->second.down && common::GetEpoch() - it->second.released_at <= time_released_ms;
    }

    return false;
  }

  void Keyboard::SetKeyState(vk_key_t key, bool state) {
    if (!IsKeyboardInput(key))
      return;

    misc::ScopedSpinlock lock(lock_);
    if (state) {
      key_state_[static_cast<INPUT>(key)] = {.down = true};
      return;
    }

    key_state_[static_cast<INPUT>(key)] = {false, common::GetEpoch()};
  }
  void Keyboard::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (!kKEYBOARD)
      return;

    switch (message) {
      case WM_SYSKEYDOWN:
      case WM_KEYDOWN:
        kKEYBOARD->SetKeyState(wParam, true);
        break;
      case WM_SYSKEYUP:
      case WM_KEYUP:
        kKEYBOARD->SetKeyState(wParam, false);
        break;
      default:
        break;
    }
  }
}