//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class NumberRangeComponent : public BaseComponent {
    public:

    NumberRangeComponent(const std::string& name, std::shared_ptr<std::atomic<T>> optVal, const T& optMin, const T& optMax, const T& optStep) :
    BaseComponent(), optVal_{optVal}, optMin_{optMin}, optMax_{optMax}, optStep_{optStep} {
      left_text_ = name;
    }

    virtual std::string GetRightText() const {
      const T val = optVal_->load();
      const std::string numVal = GetNumAsString(val);

      const bool isMin = val == optMin_;
      const bool isMax = val == optMax_;

      return fmt::format("{} {} {}", isMin ? "" : ICON_FA_ANGLE_LEFT, numVal, isMax ? "" : ICON_FA_ANGLE_RIGHT);
    }

    virtual bool HasRightText() const override {
      return true;
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kLEFT) {
        T curVal = optVal_->load();
        if (curVal == optMin_) {
          return;
        }

        T newVal = curVal - optStep_;
        if (newVal < optMin_) {
          newVal = optMin_;
        }
        optVal_->store(newVal);
      } else if (button == PressedButton::kRIGHT) {
        T curVal = optVal_->load();
        if (curVal == optMax_) {
          return;
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
