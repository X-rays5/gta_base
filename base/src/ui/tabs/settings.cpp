//
// Created by X-ray on 09/18/22.
//

#include "settings.hpp"
#include "tab_includes.hpp"
#include "../../misc/hotkey_manager.hpp"

namespace gta_base::ui::tabs {
  static std::uint64_t hotkey_delete_key_id = 0;

  void SettingsTab() {
    kMANAGER->AddSubmenu(Submenus::Settings, "tab/title/setting", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/hotkeys", "", Submenus::SettingsHotkeys));
      sub->AddOption(option::SubmenuOption("tab/title/theme", "", Submenus::SettingsTheme));
      sub->AddOption(option::SubmenuOption("tab/title/unload", "", Submenus::SettingsUnloadConfirm));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsHotkeys, "tab/title/hotkeys", [](Submenu* sub){
      for (auto&& hotkey : misc::kHOTKEY_MANAGER->GetAllHotkeys()) {
        sub->AddOption(option::SubmenuOption(hotkey.second, "", Submenus::HotkeysConfirmDelete, [=]{
          hotkey_delete_key_id = hotkey.first;
        }, false))->SetRightTextKey(common::VkToStr(hotkey.first));
      }
    });

    kMANAGER->AddSubmenu(Submenus::HotkeysConfirmDelete, ("tab/title/delete_hotkey"), [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("confirm/yes", "", []{
        misc::kHOTKEY_MANAGER->RemoveHotkey(hotkey_delete_key_id);
        kMANAGER->PopSubmenu();
      }));
      sub->AddOption(option::ExecuteOption("confirm/no", "", []{
        kMANAGER->PopSubmenu();
      }));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsTheme, "tab/title/theme", [](Submenu* sub){
      sub->AddOption(option::NumberOption<float>("option/xpos", "option/xpos/desc", &kMANAGER->x_base, 0.005f, 0.f, 1.f - kMANAGER->x_size));
      sub->AddOption(option::NumberOption<float>("option/ypos", "option/ypos/desc", &kMANAGER->y_base, 0.005f, 0.f, 1.f));
      sub->AddOption(option::NumberOption<int>("option/max_options", "option/max_options/desc", &kMANAGER->max_drawn_options, 1, 1, 30));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsUnloadConfirm, "tab/title/unload", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
        globals::running = false;
      }));
      sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
        kMANAGER->PopSubmenu();
      }));
    });
  }
}