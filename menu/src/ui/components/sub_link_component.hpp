//
// Created by X-ray on 01/06/2025.
//

#ifndef SUB_LINK_COMPONENT_HPP_06151948
#define SUB_LINK_COMPONENT_HPP_06151948
#include <functional>
#include <imfont/IconsFontAwesome6.hpp>
#include "base_component.hpp"

#include "../menu_renderer.hpp"

namespace base::menu::ui::components {
  template <typename Func = std::function<void()>>
  class SubLinkComponent final : public BaseComponent {
  public:
    explicit SubLinkComponent(const std::string& navigate_to) : navigation_link_(navigate_to), handler_() {
      const auto sub = kMENU_RENDERER->GetSubmenu(navigate_to);
      if (sub) {
        left_text_ = sub->GetName();
      } else {
        left_text_ = navigate_to;
      }

      right_text_ = ICON_FA_ARROW_RIGHT;
    }

    explicit SubLinkComponent(const SubmenuIDs navigate_to) : SubLinkComponent(std::string(magic_enum::enum_name(navigate_to)), Func()) {}

    SubLinkComponent(const std::string& navigate_to, const Func handler) : navigation_link_(navigate_to), handler_(handler) {
      const auto sub = kMENU_RENDERER->GetSubmenu(navigate_to);
      if (sub) {
        left_text_ = sub->GetName();
      } else {
        left_text_ = navigate_to;
      }

      right_text_ = ICON_FA_ARROW_RIGHT;
    }

    explicit SubLinkComponent(const SubmenuIDs navigate_to, const Func handler) : SubLinkComponent(std::string(magic_enum::enum_name(navigate_to)), handler) {}

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        util::kTHREAD_POOL->emplace_back([handler = handler_, this] {
          handler();
        });
        NavigateToSubmenu();
      }
    }

  private:
    const std::string navigation_link_;
    Func handler_;

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
