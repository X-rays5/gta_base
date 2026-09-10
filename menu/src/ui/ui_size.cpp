//
// Created by X-ray on 10/09/2026.
//

#include "menu_renderer.hpp"

namespace base::menu::ui {
  UiSize::operator float() const {
    return Scaled();
  }

  std::float_t UiSize::Scaled() const {
    std::float_t ui_scale = 1;
    if (kMENU_RENDERER) {
      ui_scale = kMENU_RENDERER->GetCurrentUiScale();
    }

    return value * ui_scale;
  }

  constexpr std::float_t UiSize::Raw() const {
    return value;
  }
}
