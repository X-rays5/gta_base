//
// Created by X-ray on 09/18/22.
//

#include "weapon.hpp"

namespace gta_base::looped_game {
void InfiniteAmmo() {
  if (kSETTINGS.player.weapon.infinite_ammo) {
    Hash wep;
    if (WEAPON::GET_CURRENT_PED_WEAPON(globals::local_player.ped_id, &wep, true)) {
      if (WEAPON::GET_AMMO_IN_PED_WEAPON(globals::local_player.ped_id, wep) == 0) {
        WEAPON::SET_PED_AMMO(globals::local_player.ped_id, wep, 1, true);
      }
    }
  }

  WEAPON::SET_PED_INFINITE_AMMO(globals::local_player.ped_id, kSETTINGS.player.weapon.infinite_ammo, NULL);
}

void InfiniteClip() {
  if (kSETTINGS.player.weapon.infinite_clip) {
    Hash wep;
    if (WEAPON::GET_CURRENT_PED_WEAPON(globals::local_player.ped_id, &wep, true)) {
      int ammo = 0;
      WEAPON::GET_AMMO_IN_CLIP(globals::local_player.ped_id, wep, &ammo);
      if (ammo == 0) {
        WEAPON::SET_AMMO_IN_CLIP(globals::local_player.ped_id, wep, 1);
      }
    }
  }

  WEAPON::SET_PED_INFINITE_AMMO_CLIP(globals::local_player.ped_id, kSETTINGS.player.weapon.infinite_ammo);
}

void Weapons() {
  InfiniteAmmo();
  InfiniteClip();
}
}