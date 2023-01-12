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
    Vehicle,
    VehicleSpawner,
    VehicleSpawnerSelectedCategory,
    Misc,
    Settings,
    SettingsLua,
    LuaScriptList,
    LuaSelectedScript,
    SettingsTranslation,
    TranslationSelected,
    TranslationConfirmDelete,
    SettingsTheme,
    SettingsHotkeys,
    HotkeyProfiles,
    HotkeysConfirmDelete,
    SettingsUnloadConfirm,
    Debug,
    DebugTestComponents,
    DebugLong,
  };

  enum class OptionFlag {
    kDisabled,
    kRightText,
    kSubmenuLink,
    kHotkeyable,
    kLabel,
    kToggle,
    kToggled,
    kSaveable,
    kInput
  };

  enum class KeyInput {
    kUp,
    kDown,
    kLeft,
    kRight,
    kReturn,
    kBackspace,
    kHotkey,
    kChangeValue,
  };

  enum class Event {
    kSelect,
    kChange
  };
}
#endif //GTA_BASE_ENUMS_HPP