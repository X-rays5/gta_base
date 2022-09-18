//
// Created by X-ray on 09/18/22.
//

#include "main_tab.hpp"
#include "tab_includes.hpp"
#include "debug.hpp"
#include "misc.hpp"
#include "selected_player.hpp"
#include "self.hpp"
#include "settings.hpp"
#include "world.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      void MainTab() {
        kMANAGER->AddSubmenu(Submenus::Home, "tab/title/home", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("tab/title/self", "", Submenus::Player));
          if (common::IsSessionStarted()) {
            sub->AddOption(option::SubmenuOption("tab/title/player_list", "", Submenus::PlayerList));
          }
#ifndef NDEBUG
          sub->AddOption(option::SubmenuOption("Debug", "", Submenus::Debug));
#endif
          sub->AddOption(option::SubmenuOption("tab/title/misc", "", Submenus::Misc));
          sub->AddOption(option::SubmenuOption("tab/title/setting", "", Submenus::Settings));
        });

        DebugTab();
        MiscTab();
        SelectedPlayerTab();
        SelfTab();
        SettingsTab();
        WorldTab();
      }
    }
  }
}