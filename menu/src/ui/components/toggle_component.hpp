//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  class ToggleComponent final : public BaseComponent {
    public:
    ToggleComponent(const std::string& name, const std::shared_ptr<std::atomic<bool>>& optState) {
      BaseComponent();
      left_text_ = name;
      optState_ = optState;
    }

    virtual std::string GetRightText() const override {
      return optState_ && *optState_ ? ICON_FA_CHECK : ICON_FA_XMARK;
    }

    virtual bool HasRightText() const override {
      return true;
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        optState_->store(!optState_->load());
      }
    }

  private:
    std::shared_ptr<std::atomic<bool>> optState_;
  };
}