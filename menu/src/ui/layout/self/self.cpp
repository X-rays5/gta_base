//
// Created by X-ray on 27/03/2026.
//

#include "self.hpp"
#include "../../../options/option_registry.hpp"
#include "../../components/components.hpp"

namespace base::menu::ui::layout {
  void InitHealthSub() {
    Submenu health_submenu("ui/sub/self_health", [](Submenu* sub) {
      if (options::kOPTION_REGISTRY) {
        const auto options = options::kOPTION_REGISTRY->options().lock();

        options->god_mode_option->CreateOptionUi("ui/option/self_godmode", sub);
        options->semi_god_mode_option->CreateOptionUi("ui/option/self_semigodmode", sub);
        options->self_heal_option->CreateOptionUi("ui/action/self_heal", sub);
        options->suicide_option->CreateOptionUi("ui/action/suicide", sub);
        options->self_max_armour_option->CreateOptionUi("ui/action/self_max_armour", sub);
        options->self_remove_armour_option->CreateOptionUi("ui/action/self_remove_armour", sub);
      }
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSELF_HEALTH, std::move(health_submenu));
  }

  void InitSelfLayout() {
    InitHealthSub();

    Submenu self_submenu("ui/sub/self", [](Submenu* sub) {
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kSELF_HEALTH));
      if (options::kOPTION_REGISTRY) {
        const auto options = options::kOPTION_REGISTRY->options().lock();

        options->force_wanted_level_option->CreateOptionUi("ui/option/force_wanted_level", sub);
        options->never_wanted_option->CreateOptionUi("ui/option/never_wanted", sub);
      }
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSELF, std::move(self_submenu));
  }
}
