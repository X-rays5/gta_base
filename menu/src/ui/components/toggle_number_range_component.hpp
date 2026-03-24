//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class ToggleNumberRangeComponent : public BaseComponent {
    public:

    ToggleNumberRangeComponent(const std::string& name, std::shared_ptr<std::atomic<T>> optVal, const T& optMin, const T& optMax, const T& optStep, const std::shared_ptr<std::atomic<bool>>& toggle_state) :
    BaseComponent(), optVal_{optVal}, optMin_{optMin}, optMax_{optMax}, optStep_{optStep}, toggle_state_(toggle_state) {
      left_text_ = name;
    }

    virtual std::string GetRightText() const override {
      const T val = optVal_->load();
      const std::string numVal = GetNumAsString(val);

      const bool isMin = val == optMin_;
      const bool isMax = val == optMax_;
      const bool is_enabled = toggle_state_ && toggle_state_->load();

      return fmt::format("{} {} {} {}", isMin ? "" : ICON_FA_ANGLE_LEFT, numVal, isMax ? "" : ICON_FA_ANGLE_RIGHT, is_enabled ? ICON_FA_CHECK : ICON_FA_XMARK);
    }

    virtual bool HasRightText() const override {
      return true;
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        // Toggle the boolean state
        if (toggle_state_) {
          toggle_state_->store(!toggle_state_->load());
        }
      } else if (button == PressedButton::kLEFT) {
        // Decrease value (no wrapping)
        T curVal = optVal_->load();
        if (curVal == optMin_) {
          return; // Already at minimum
        }

        T newVal = curVal - optStep_;
        if (newVal < optMin_) {
          newVal = optMin_;
        }
        optVal_->store(newVal);
      } else if (button == PressedButton::kRIGHT) {
        // Increase value (no wrapping)
        T curVal = optVal_->load();
        if (curVal == optMax_) {
          return; // Already at maximum
        }

        T newVal = curVal + optStep_;
        if (newVal > optMax_) {
          newVal = optMax_;
        }
        optVal_->store(newVal);
      }
    }

  private:
    std::shared_ptr<std::atomic<T>> optVal_;
    const T optMin_;
    const T optMax_;
    const T optStep_;
    std::shared_ptr<std::atomic<bool>> toggle_state_;

  private:

    std::string GetNumAsString(T val) const {
        if constexpr (std::is_integral_v<T>) {
          return std::to_string(val);
        } else if constexpr (std::is_floating_point_v<T>) {
          return fmt::format("{:.3f}", val);
        }
    }
  };
}

