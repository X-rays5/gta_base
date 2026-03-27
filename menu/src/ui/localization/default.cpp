//
// Created by X-ray on 29/12/2023.
//
#include "manager.hpp"

#define ADD_TRANSLATION(key, trans) {xorstr_(key), xorstr_(trans)},

namespace base::menu::ui::localization {
  const translation_map_t default_translation = {
    ADD_TRANSLATION("text/hello_world", "Hello World!")
    ADD_TRANSLATION("label/ok", "Ok")
    ADD_TRANSLATION("label/cancel", "Cancel")
    ADD_TRANSLATION("label/yes", "Yes")
    ADD_TRANSLATION("label/no", "No")
    ADD_TRANSLATION("label/save", "Save")
    ADD_TRANSLATION("label/load", "Load")
    ADD_TRANSLATION("label/confirm", "Confirm")
    ADD_TRANSLATION("label/reset", "Cancel")
    ADD_TRANSLATION("label/invalid_submenu", "Invalid Submenu")

    ADD_TRANSLATION("ui/sub/home", "Home")
    ADD_TRANSLATION("ui/sub/self", "Self")
    ADD_TRANSLATION("ui/sub/self_health", "Health")
    ADD_TRANSLATION("ui/option/self_godmode", "God Mode")
    ADD_TRANSLATION("ui/option/self_semigodmode", "Semi-God Mode")
    ADD_TRANSLATION("ui/action/self_heal", "Heal")
    ADD_TRANSLATION("ui/action/suicide", "Suicide")
    ADD_TRANSLATION("ui/action/self_max_armour", "Max Armour")
    ADD_TRANSLATION("ui/action/self_remove_armour", "Remove Armour")
    ADD_TRANSLATION("ui/option/force_wanted_level", "Force Wanted Level")
    ADD_TRANSLATION("ui/option/never_wanted", "Never Wanted")
    ADD_TRANSLATION("ui/sub/settings", "Settings")
    ADD_TRANSLATION("ui/sub/unload", "Unload")
    ADD_TRANSLATION("ui/sub/theme", "Theme")
    ADD_TRANSLATION("ui/option/ui_x_pos", "UI X Position")
    ADD_TRANSLATION("ui/option/ui_y_pos", "UI Y Position")

    ADD_TRANSLATION("ui/sub/feature_settings", "Feature Settings")
  };
}
