//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_SETTINGS_HPP
#define GTA_BASE_SETTINGS_HPP
#include <string>
#include <robin_hood.h>


namespace gta_base {
  struct SettingsPlayer {
    bool god_mode = false;
    bool semi_god_mode = false;
  };

  struct Settings {
   SettingsPlayer player;
  };

  extern Settings kSETTINGS;
}
#endif //GTA_BASE_SETTINGS_HPP
