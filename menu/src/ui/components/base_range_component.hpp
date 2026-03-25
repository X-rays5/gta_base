//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  /**
   * Base class for range-based components to reduce code duplication.
   * Handles common range navigation logic (LEFT/RIGHT arrows for value adjustment).
   */
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class BaseRangeComponent : public BaseComponent {
  protected:
    std::shared_ptr<std::atomic<T>> optVal_;
    const T optMin_;
    const T optMax_;
    const T optStep_;

  public:
    BaseRangeComponent(const std::string& name, std::shared_ptr<std::atomic<T>> optVal, const T& optMin, const T& optMax, const T& optStep)
        : optVal_(optVal), optMin_(optMin), optMax_(optMax), optStep_(optStep) {
      left_text_ = name;
    }

    virtual ~BaseRangeComponent() = default;

    /**
     * Get the current value
     */
    T GetCurrentValue() const {
      return optVal_->load();
    }

    /**
     * Check if at minimum value
     */
    bool IsAtMin() const {
      return GetCurrentValue() == optMin_;
    }

    /**
     * Check if at maximum value
     */
    bool IsAtMax() const {
      return GetCurrentValue() == optMax_;
    }

    /**
     * Decrease value by step (no wrapping)
     */
    void DecreaseValue() {
      T curVal = GetCurrentValue();
      if (curVal == optMin_) {
        return; // Already at minimum
      }

      T newVal = curVal - optStep_;
      if (newVal < optMin_) {
        newVal = optMin_;
      }
      optVal_->store(newVal);
    }

    /**
     * Increase value by step (no wrapping)
     */
    void IncreaseValue() {
      T curVal = GetCurrentValue();
      if (curVal == optMax_) {
        return; // Already at maximum
      }

      T newVal = curVal + optStep_;
      if (newVal > optMax_) {
        newVal = optMax_;
      }
      optVal_->store(newVal);
    }

    /**
     * Get the current value as a string
     */
    std::string GetValueAsString() const {
      const T val = GetCurrentValue();
      if constexpr (std::is_integral_v<T>) {
        return std::to_string(val);
      } else if constexpr (std::is_floating_point_v<T>) {
        return fmt::format("{:.3f}", val);
      }
    }

    /**
     * Get formatted arrow indicators for navigation
     */
    std::string GetNavigationArrows() const {
      return fmt::format("{}{}", IsAtMin() ? "" : ICON_FA_ANGLE_LEFT, IsAtMax() ? "" : ICON_FA_ANGLE_RIGHT);
    }

    virtual bool HasRightText() const override {
      return true;
    }

    /**
     * Default button press handling for range navigation
     * Derived classes should call this or override with their own logic
     */
    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kLEFT) {
        DecreaseValue();
      } else if (button == PressedButton::kRIGHT) {
        IncreaseValue();
      }
    }
  };
}

