//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_ENUMS_UI_HPP
#define GTA_BASE_ENUMS_UI_HPP

namespace gta_base::ui {
    enum class Submenus {
      Home,
      Player,
      PlayerHealth,
      Network,
      NetworkPlayerList,
      NetworkAllPlayers,
      NetworkSessionSwitcher,
      NetworkRequestServices,
      PlayerListSelectedPlayer,
      Weapon,
      Misc,
      Settings,
      SettingsTheme,
      SettingsHotkeys,
      SettingsUnloadConfirm,
      Debug,
      DebugTestComponents,
    };

    enum class OptionFlag {
      kDisabled,
      kRightIcon,
      kRightText,
      kSubmenuLink,
      kHotkeyable,
      kLabel,
      kToggle,
      kToggled
    };

    enum class KeyInput {
      kUp,
      kDown,
      kLeft,
      kRight,
      kReturn,
      kBackspace,
      kHotkey,
    };

    enum class Event {
      kSelect,
      kChange
    };
  }
#endif //GTA_BASE_ENUMS_HPP