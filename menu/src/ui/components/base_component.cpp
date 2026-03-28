//
// Created by X-ray on 28/03/2026.
//

#include "base_component.hpp"
#include "../../options/base_option.hpp"

namespace base::menu::ui::components {
  bool BaseComponent::IsHotkeyAble() const {
    return option_ ? option_->IsHotkeyAble() : false;
  }

  bool BaseComponent::IsSavable() const {
    return option_ ? option_->IsSavable() : false;
  }
}
