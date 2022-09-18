//
// Created by X-ray on 09/18/22.
//

#include "main_tab.hpp"
#include "tab_includes.hpp"
#include "debug.hpp"
#include "misc.hpp"
#include "network.hpp"
#include "self.hpp"
#include "settings.hpp"
#include "world.hpp"
#include "weapon.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      void MainTab() {
        kMANAGER->AddSubmenu(Submenus::Home, "tab/title/home", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("tab/title/self", "", Submenus::Player));
          sub->AddOption(option::SubmenuOption("tab/title/weapon", "", Submenus::Weapon));
          sub->AddOption(option::SubmenuOption("tab/title/network", "", Submenus::Network));
          sub->AddOption(option::SubmenuOption("tab/title/misc", "", Submenus::Misc));
          sub->AddOption(option::SubmenuOption("tab/title/setting", "", Submenus::Settings));
#ifndef NDEBUG
          sub->AddOption(option::SubmenuOption("Debug", "", Submenus::Debug));
#endif
        });

        DebugTab();
        MiscTab();
        NetworkTab();
        SelfTab();
        SettingsTab();
        WorldTab();
        WeaponsTab();
      }
    }
  }
}