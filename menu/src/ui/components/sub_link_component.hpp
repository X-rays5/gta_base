//
// Created by X-ray on 01/06/2025.
//

#ifndef SUB_LINK_COMPONENT_HPP_06151948
#define SUB_LINK_COMPONENT_HPP_06151948
#include <imfont/IconsFontAwesome6.hpp>
#include "base_component.hpp"

#include "../menu_renderer.hpp"

namespace base::menu::ui::components {
  class SubLinkComponent final : public BaseComponent {
  public:
    explicit SubLinkComponent(const std::string& navigate_to) : BaseComponent(), navigation_link_(navigate_to) {
      left_text_ = navigate_to;
      right_text_ = ICON_FA_ARROW_RIGHT;
    }
    explicit SubLinkComponent(const SubmenuIDs navigate_to) : SubLinkComponent(std::string(magic_enum::enum_name(navigate_to))) {}

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT || button == PressedButton::kRIGHT) {
        NavigateToSubmenu();
      }
    }

  private:
    const std::string navigation_link_;

  private:
    void NavigateToSubmenu() const {
      if (!navigation_link_.empty() && kMENU_RENDERER) {
        if (auto res = kMENU_RENDERER->PushSubmenu(navigation_link_); !res) {
          LOG_ERROR("Failed to push sub: {}", res);
        }
      } else {
        LOG_ERROR("Navigation link is empty or MenuRenderer is not initialized.");
      }
    }
  };
}

#endif //SUB_LINK_COMPONENT_HPP_06151948
