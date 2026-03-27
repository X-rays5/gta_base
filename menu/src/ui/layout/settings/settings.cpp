//
// Created by X-ray on 26/03/2026.
//

#include "settings.hpp"
#include "../../menu_renderer.hpp"
#include "../../components/components.hpp"
#include "../../../feature/feature_settings.hpp"
#include "../../../options/option_registry.hpp"

namespace base::menu::ui::layout {
  void ThemeSub() {
    // Get theme reference that captures the shared_ptr to keep it alive
    auto menuTheme = kMENU_RENDERER->GetTheme();

    Submenu theme_submenu("ui/sub/theme", [menuTheme] (Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", [menuTheme] {
        auto res = menuTheme->Save("default");
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", [menuTheme] {
        auto res = menuTheme->Load("default");
      }));
      // Pass references directly to theme member variables so changes affect the actual theme object
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_x_pos", menuTheme->x_position, 0.005F));
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_y_pos", menuTheme->y_position, 0.005F));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kTHEME_SETTINGS, std::move(theme_submenu));
  }

  void FeatureSettingsSub() {
    Submenu feature_submenu("ui/sub/feature_settings", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", [] {
        if (!feature::kSETTINGS)
          return;

        auto status = feature::kSETTINGS->Save("default");
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->SaveOptions("default");
        }
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", [] {
        if (!feature::kSETTINGS)
          return;

        auto status = feature::kSETTINGS->Load("default");
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->LoadOptions("default");
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLOAD_FEATURE_SETTINGS, std::move(feature_submenu));
  }

  void UnloadConfirmSub() {
    Submenu unload_sub("ui/sub/unload", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/confirm", [] {
        globals::kRUNNING = false;
      }));
      sub->AddComponent(components::ExecuteComponent("label/cancel", [] {
        if (kMENU_RENDERER) {
          kMENU_RENDERER->PopSubmenu();
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kUNLOAD_CONFIRM, std::move(unload_sub));
  }

  void InitSettingsLayout() {
    Submenu settings_submenu("ui/sub/settings", [](Submenu* sub) {
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kTHEME_SETTINGS));
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kLOAD_FEATURE_SETTINGS));
#ifndef NDEBUG
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kUNLOAD_CONFIRM));
#endif
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSETTINGS, std::move(settings_submenu));

    ThemeSub();
    FeatureSettingsSub();

#ifndef NDEBUG
    UnloadConfirmSub();
#endif
  }
}