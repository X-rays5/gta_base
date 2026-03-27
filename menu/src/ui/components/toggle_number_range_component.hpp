//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_range_component.hpp"

namespace base::menu::ui::components {
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class ToggleNumberRangeComponent final : public BaseRangeComponent<T> {
  public:
    using BaseComponent::PressedButton;

    ToggleNumberRangeComponent(const std::string& name, const std::string& description, std::atomic<T>* optVal, const T& optMin, const T& optMax,
                               const T& optStep, std::atomic<bool>* toggle_state)
        : BaseRangeComponent<T>(name, description, optVal, optMin, optMax, optStep), toggle_state_(toggle_state) {
    }

    virtual std::string GetRightText() const override {
      const bool is_enabled = toggle_state_ && toggle_state_->load();
      return fmt::format("{} {} {}", this->GetValueAsString(), this->GetNavigationArrows(), is_enabled ? ICON_FA_CHECK : ICON_FA_XMARK);
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        // Toggle the boolean state
        if (toggle_state_) {
          toggle_state_->store(!toggle_state_->load());
        }
      } else {
        // Use parent's navigation logic for LEFT/RIGHT
        BaseRangeComponent<T>::HandleButtonPress(button);
      }
    }

  private:
    std::atomic<bool>* toggle_state_;
  };
}

