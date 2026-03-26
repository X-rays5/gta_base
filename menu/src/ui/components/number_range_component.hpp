//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_range_component.hpp"

namespace base::menu::ui::components {
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class NumberRangeComponent final : public BaseRangeComponent<T> {
  public:
    NumberRangeComponent(const std::string& name, std::shared_ptr<std::atomic<T>> optVal, const T& optMin, const T& optMax, const T& optStep)
        : BaseRangeComponent<T>(name, optVal, optMin, optMax, optStep) {
    }

    NumberRangeComponent(const std::string& name, T& optVal, const T& optMin, const T& optMax, const T& optStep)
        : BaseRangeComponent<T>(name, std::make_shared<std::atomic<T>>(optVal), optMin, optMax, optStep) {
    }

    template <T min_val, T max_val>
    NumberRangeComponent(const std::string& name, common::util::RangedValue<T, min_val, max_val>& optVal, const T& optStep = static_cast<T>(1))
        : BaseRangeComponent<T>(name, std::make_shared<std::atomic<T>>(static_cast<T>(optVal)), min_val, max_val, optStep) {
      // Store pointer to actual RangedValue for direct modification
      this->SetRangedValuePtr(&optVal);
    }

    virtual std::string GetRightText() const override {
      return fmt::format("{} {}", this->GetValueAsString(), this->GetNavigationArrows());
    }
  };
}
