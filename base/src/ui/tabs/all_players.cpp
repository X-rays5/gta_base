//
// Created by X-ray on 09/18/22.
//

#include "all_players.hpp"
#include "tab_includes.hpp"

namespace gta_base::ui::tabs {
  void AllPlayersTab() {
    kMANAGER->AddSubmenu(Submenus::NetworkAllPlayers, "tab/title/all_players", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("placeholder"));
    });
  }
}