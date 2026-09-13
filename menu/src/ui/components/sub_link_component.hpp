//
// Created by X-ray on 01/06/2025.
//

#ifndef SUB_LINK_COMPONENT_HPP_06151948
#define SUB_LINK_COMPONENT_HPP_06151948
#include <functional>
#include <string>
#include <utility>
#include <imfont/IconsFontAwesome6.hpp>
#include "base_component.hpp"

#include "../menu_renderer.hpp"

namespace base::menu::ui::components {
  template <typename Func = std::function<void()>>
  class SubLinkComponent final : public BaseComponent {
  public:
    /**
     * The row's text is the caller's to name, and the renderer is never touched. A link whose target
     * is a script's submenu has a key rather than an id to read it by, and a link built where no
     * renderer exists at all - a test harness - has nobody to ask. The constructors below resolve a
     * renderer id into the submenu's own name and come through here.
     *
     * Two of them rather than one with a defaulted handler: Func is whatever the handler's type is
     * deduced to be, which for a lambda is a type with no empty value, and a default argument would
     * have to name one.
     */
    SubLinkComponent(std::string navigate_to, std::string name_key) : navigation_link_(std::move(navigate_to)) {
      left_text_ = std::move(name_key);
      right_text_ = ICON_FA_ARROW_RIGHT;
    }

    SubLinkComponent(std::string navigate_to, std::string name_key, Func handler)
        : navigation_link_(std::move(navigate_to)), handler_(std::move(handler)) {
      left_text_ = std::move(name_key);
      right_text_ = ICON_FA_ARROW_RIGHT;
    }

    explicit SubLinkComponent(const std::string& navigate_to) : SubLinkComponent(navigate_to, ResolveName(navigate_to)) {}

    explicit SubLinkComponent(const SubmenuIDs navigate_to) : SubLinkComponent(std::string(enchantum::to_string(navigate_to))) {}

    SubLinkComponent(const std::string& navigate_to, const Func handler) : SubLinkComponent(navigate_to, ResolveName(navigate_to), handler) {}

    explicit SubLinkComponent(const SubmenuIDs navigate_to, const Func handler) : SubLinkComponent(std::string(enchantum::to_string(navigate_to)), handler) {}

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        util::kTHREAD_POOL->emplace_back([handler = handler_] {
          if constexpr (std::is_same_v<Func, std::function<void()>>) {
            if (handler)
              handler();
          } else {
            handler();
          }
        });
        NavigateToSubmenu();
      }
    }

    void SetName(const std::string& name) {
      left_text_ = name;
    }

    void SetDescription(const std::string& description) {
      description_ = description;
    }

  private:
    const std::string navigation_link_;
    Func handler_;

  private:
    /**
     * The submenu's own name, or the id as written when there is no renderer or no such submenu. Both
     * misses are ordinary rather than errors: a harness has no renderer, and a link to a submenu that
     * is registered later still has to read as something until it is.
     */
    static std::string ResolveName(const std::string& navigate_to) {
      const auto sub = kMENU_RENDERER ? kMENU_RENDERER->GetSubmenu(navigate_to) : nullptr;
      return sub ? sub->GetName() : navigate_to;
    }

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
