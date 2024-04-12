//
// Created by X-ray on 29/12/2023.
//
#include "manager.hpp"

#define ADD_TRANSLATION(key, trans) {xorstr_(key), xorstr_(trans)}

namespace base::ui::localization {
  const translation_map_t default_translation = {
    ADD_TRANSLATION("text/hello_world", "Hello World!")
  };
}
