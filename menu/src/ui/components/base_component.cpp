//
// Created by X-ray on 28/03/2026.
//

#include "base_component.hpp"
#include "../../options/base_option.hpp"
#include "../../options/option_registry.hpp"

namespace base::menu::ui::components {
  bool BaseComponent::IsHotkeyAble() const {
    return option_ ? option_->IsHotkeyAble() : false;
  }

  bool BaseComponent::IsSavable() const {
    return option_ ? option_->IsSavable() : false;
  }

  void BaseComponent::Save() const {
    if (!IsSavable() && !option_) {
      LOG_ERROR("Component '{}' is not savable and has no associated option, cannot save", GetLeftText());
      return;
    }

    auto status = options::kOPTION_REGISTRY->SaveOption(option_);
    if (!status) {
      LOG_ERROR("Failed to save option for component '{}': {}", GetLeftText(), status);
    }
  }
}
