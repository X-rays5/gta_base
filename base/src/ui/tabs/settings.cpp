//
// Created by X-ray on 09/18/22.
//

#include "settings.hpp"
#include "tab_includes.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      void SettingsTab() {
        kMANAGER->AddSubmenu(Submenus::Settings, "tab/title/setting", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("tab/title/theme", "", Submenus::Theme));
          sub->AddOption(option::SubmenuOption("tab/title/unload", "", Submenus::UnloadConfirm));
        });

        kMANAGER->AddSubmenu(Submenus::Theme, "tab/title/theme", [](Submenu* sub){
          sub->AddOption(option::NumberOption<float>("option/xpos", "option/xpos/desc", &kMANAGER->x_base, 0.005f, 0.f, 1.f - kMANAGER->x_size));
          sub->AddOption(option::NumberOption<float>("option/ypos", "option/ypos/desc", &kMANAGER->y_base, 0.005f, 0.f, 1.f));
          sub->AddOption(option::NumberOption<int>("option/max_options", "option/max_options/desc", &kMANAGER->max_drawn_options, 1, 1, 30));
        });

        kMANAGER->AddSubmenu(Submenus::UnloadConfirm, "tab/title/unload", [](Submenu* sub) {
          sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
            globals::running = false;
          }));
          sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
            kMANAGER->PopSubmenu();
          }));
        });
      }
    }
  }
}