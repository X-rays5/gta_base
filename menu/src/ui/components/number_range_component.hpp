//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_range_component.hpp"

namespace base::menu::ui::components {
  template <typename T>
   requires std::is_integral_v<T> or std::is_floating_point_v<T>
  class NumberRangeComponent final : public BaseRangeComponent<T> {
  private:
    std::shared_ptr<std::atomic<T>> shared_atomic_;  // Keep shared_ptr alive for managed lifetime

  public:
    NumberRangeComponent(const std::string& name, std::shared_ptr<std::atomic<T>> optVal, const T& optMin, const T& optMax, const T& optStep)
        : BaseRangeComponent<T>(name, optVal.get(), optMin, optMax, optStep), shared_atomic_(optVal) {
    }

    NumberRangeComponent(const std::string& name, T& optVal, const T& optMin, const T& optMax, const T& optStep)
        : shared_atomic_(std::make_shared<std::atomic<T>>(optVal)),
          BaseRangeComponent<T>(name, shared_atomic_.get(), optMin, optMax, optStep) {
    }

    template <T min_val, T max_val>
    NumberRangeComponent(const std::string& name, common::util::RangedValue<T, min_val, max_val>& optVal, const T& optStep = static_cast<T>(1))
        : shared_atomic_(std::make_shared<std::atomic<T>>(static_cast<T>(optVal))),
          BaseRangeComponent<T>(name, shared_atomic_.get(), min_val, max_val, optStep) {
      // Store pointer to actual RangedValue for direct modification
      this->SetRangedValuePtr(&optVal);
    }

    virtual std::string GetRightText() const override {
      return fmt::format("{} {}", this->GetValueAsString(), this->GetNavigationArrows());
    }
  };
}
