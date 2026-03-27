//
// Created by X-ray on 26/03/2026.
//

#include "home.hpp"

#include "../menu_renderer.hpp"
#include "../components/components.hpp"
#include "self/self.hpp"
#include "vehicle/vehicle.hpp"
#include "settings/settings.hpp"

namespace base::menu::ui::layout {
  void InitHomeLayout() {
    Submenu home_submenu("ui/sub/home", [](Submenu* sub) {
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kSELF));
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kVEHICLE));
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kSETTINGS));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kMAIN_MENU, std::move(home_submenu));

    InitSelfLayout();
    InitVehicleLayout();
    InitSettingsLayout();
  }
}
