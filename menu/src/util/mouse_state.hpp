#pragma once

namespace base::menu::util::input {
  /**
   * @class MouseState
   * @brief Tracks mouse input state similar to KeyState
   *
   * Provides thread-safe access to mouse click and wheel events.
   * Events are consumed immediately upon reading.
   */
  class MouseState {
  public:
    /// Check if left mouse button was clicked (consumed immediately)
    bool WasLeftClicked();

    /// Check if right mouse button was clicked (consumed immediately)
    bool WasRightClicked();

    /// Get mouse wheel delta (consumed immediately)
    float GetWheelDelta();

    /// Called from WndProc listener to record mouse events
    void OnMouseLeftClick();
    void OnMouseRightClick();
    void OnMouseWheel(float delta);

  private:
    bool left_clicked_ = false;
    bool right_clicked_ = false;
    float wheel_delta_ = 0.0f;
  };
}

