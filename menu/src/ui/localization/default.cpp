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
    ADD_TRANSLATION("ui/sub/settings", "Settings")

    ADD_TRANSLATION("ui/sub/unload", "Unload")

    ADD_TRANSLATION("ui/sub/theme", "Theme")
    ADD_TRANSLATION("ui/option/ui_x_pos", "UI X Position")
    ADD_TRANSLATION("ui/option/ui_y_pos", "UI Y Position")
  };
}
