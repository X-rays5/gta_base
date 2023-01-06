//
// Created by X-ray on 01/06/23.
//

#pragma once
#ifndef GTA_BASE_SETTINGS_HPP
#define GTA_BASE_SETTINGS_HPP
namespace gta_base {
  namespace settings {
    struct PlayerWeapon {
      bool infinite_ammo = false;
      bool infinite_clip = false;
    };

    struct Player {
      bool god_mode = false;
      bool semi_god_mode = false;
      bool never_wanted = false;
      PlayerWeapon weapon;
    };

    struct Menu {
      bool save_option_state_on_exit = false;
    };

    struct Settings {
      Player player;
      Menu menu;
    };
  }

  extern settings::Settings kSETTINGS;
}
#endif //GTA_BASE_SETTINGS_HPP
