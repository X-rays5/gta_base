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

    virtual std::string GetRightText() const override {
      return fmt::format("{} {}", this->GetValueAsString(), this->GetNavigationArrows());
    }
  };
}
