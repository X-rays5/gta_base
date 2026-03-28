#include "mouse_state.hpp"

namespace base::menu::util::input {
  bool MouseState::WasLeftClicked() {
    const bool result = left_clicked_;
    left_clicked_ = false;
    return result;
  }

  bool MouseState::WasRightClicked() {
    const bool result = right_clicked_;
    right_clicked_ = false;
    return result;
  }

  float MouseState::GetWheelDelta() {
    const float result = wheel_delta_;
    wheel_delta_ = 0.0f;
    return result;
  }

  void MouseState::OnMouseLeftClick() {
    left_clicked_ = true;
  }

  void MouseState::OnMouseRightClick() {
    right_clicked_ = true;
  }

  void MouseState::OnMouseWheel(float delta) {
    wheel_delta_ += delta;
  }
}

