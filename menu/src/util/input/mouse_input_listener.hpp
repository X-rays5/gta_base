#pragma once
#include <functional>
#include <windows.h>

namespace base::menu::util::input {
  /**
   * @class MouseInputListener
   * @brief Listener for mouse input events
   *
   * Similar to KeyEventListener, allows components to react to mouse events
   * without directly accessing ImGui or raw input state.
   */
  class MouseInputListener {
  public:
    virtual ~MouseInputListener() = default;

    /// Called when left mouse button is clicked
    virtual void OnMouseLeftClick() {}

    /// Called when right mouse button is clicked
    virtual void OnMouseRightClick() {}

    /// Called when mouse wheel is scrolled
    /// @param delta Wheel delta normalized to [-1, 1]
    virtual void OnMouseWheel(float) {}
  };
}

