//
// Created by X-ray on 09/18/22.
//

#include "weapon.hpp"
#include "tab_includes.hpp"

namespace gta_base::ui::tabs {
void WeaponsTab() {
  kMANAGER->AddSubmenu(Submenus::Weapon, "tab/title/weapon", [](Submenu *sub) {
    sub->AddOption(option::ToggleOption("option/infinite_ammo", "", &kSETTINGS.player.weapon.infinite_ammo));
    sub->AddOption(option::ToggleOption("option/infinite_clip", "", &kSETTINGS.player.weapon.infinite_clip));
  });
}
}