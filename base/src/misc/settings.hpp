//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_SETTINGS_HPP
#define GTA_BASE_SETTINGS_HPP
#include <string>
#include <robin_hood.h>

namespace gta_base {
  struct SettingsPlayerWeapon {
    bool infinite_ammo = false;
    bool infinite_clip = false;
  };

  struct SettingsPlayer {
    bool god_mode = false;
    bool semi_god_mode = false;
    bool never_wanted = false;
    SettingsPlayerWeapon weapon;
  };

  struct SettingsMenu {
    bool save_on_exit = true;
  };

  struct Settings {
   SettingsPlayer player;
   SettingsMenu menu;
  };

  extern Settings kSETTINGS;

  namespace settings {
    void Save();
    void Load();
  }
}
#endif //GTA_BASE_SETTINGS_HPP
