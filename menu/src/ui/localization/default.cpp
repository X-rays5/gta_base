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
    ADD_TRANSLATION("label/invalid_submenu", "Invalid Submenu")
  };
}
