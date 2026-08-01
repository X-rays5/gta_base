//
// Created by X-ray on 26/03/2026.
//

#include "settings.hpp"
#include "../../menu_renderer.hpp"
#include "../../../hotkey/hotkey_manager.hpp"
#include "../../components/components.hpp"
#include "../../../options/option_registry.hpp"

namespace {
  base::menu::hotkey::Hotkey cur_hotkey_remove;
  std::string cur_hotkey_remove_name;
}

namespace base::menu::ui::layout {
  void ThemeSub() {
    // Get theme reference that captures the shared_ptr to keep it alive
    auto menuTheme = kMENU_RENDERER->GetTheme();

    Submenu theme_submenu("ui/sub/theme", [menuTheme] (Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", "", [menuTheme] {
        auto res = menuTheme->Save("default");
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", "", [menuTheme] {
        auto res = menuTheme->Load("default");
      }));
      // Pass references directly to theme member variables so changes affect the actual theme object
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_x_pos", "", menuTheme->x_position, 0.005F));
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_y_pos", "", menuTheme->y_position, 0.005F));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kTHEME_SETTINGS, std::move(theme_submenu));
  }

  void FeatureSettingsSub() {
    Submenu feature_submenu("ui/sub/feature_settings", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", "", [] {
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->SaveOptions("default");
        }
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", "", [] {
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->LoadOptions("default");
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLOAD_FEATURE_SETTINGS, std::move(feature_submenu));
  }

  void RemoveHotkeyConfirmSub() {
    Submenu remove_hotkey_sub("ui/sub/remove_hotkey_confirm", [](Submenu* sub) {
      const std::string desc = fmt::format(fmt::runtime(localization::kMANAGER->Localize("ui/option/remove_hotkey_confirm_desc")), cur_hotkey_remove_name);
      sub->AddComponent(components::ExecuteComponent("label/confirm", desc, [] {
        if (hotkey::kHOTKEY_MANAGER) {
          LOG_DEBUG("Removing hotkey: {} - {}", cur_hotkey_remove.combined, cur_hotkey_remove.AsString());
          hotkey::kHOTKEY_MANAGER->RemoveHotkey(cur_hotkey_remove);
          kMENU_RENDERER->PopSubmenu();
        }
      }));
      sub->AddComponent(components::ExecuteComponent("label/cancel", "", [] {
        if (kMENU_RENDERER) {
          kMENU_RENDERER->PopSubmenu();
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kHOTKEY_REMOVE_CONFIRM, std::move(remove_hotkey_sub));
  }

  void HotkeysSub() {
    Submenu hotkeys_submenu("ui/sub/hotkeys", [](Submenu* sub) {
      if (hotkey::kHOTKEY_MANAGER) {
        const auto hotkeys = hotkey::kHOTKEY_MANAGER->GetAllHotkeys();
        for (const auto& [hotkey, option] : hotkeys) {
          auto comp = components::SubLinkComponent(SubmenuIDs::kHOTKEY_REMOVE_CONFIRM, [hotkey, option] {
            cur_hotkey_remove = hotkey;
            cur_hotkey_remove_name = option->GetName();
          });
          comp.SetName(option->GetName());
          comp.SetDescription(hotkey.AsString());

          sub->AddComponent(std::move(comp));
        }
      }

      if (sub->IsEmpty()) {
        sub->AddComponent(components::LabelComponent("ui/option/no_hotkeys"));
      }
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kHOTKEYS, std::move(hotkeys_submenu));

    RemoveHotkeyConfirmSub();
  }

  void UnloadConfirmSub() {
    Submenu unload_sub("ui/sub/unload", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/confirm", "", [] {
        globals::kRUNNING = false;
      }));
      sub->AddComponent(components::ExecuteComponent("label/cancel", "", [] {
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
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kHOTKEYS));
#ifndef NDEBUG
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kUNLOAD_CONFIRM));
#endif
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSETTINGS, std::move(settings_submenu));

    ThemeSub();
    FeatureSettingsSub();
    HotkeysSub();

#ifndef NDEBUG
    UnloadConfirmSub();
#endif
  }
}