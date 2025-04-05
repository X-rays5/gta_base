//
// Created by X-ray on 29/12/2023.
//
#include "manager.hpp"

#define ADD_TRANSLATION(key, trans) {xorstr_(key), xorstr_(trans)}

#define COMMON_TRANSLATION(category, ...) "common/" category "/" __VA_ARGS__
#define LABEL_TRANSLATION(...) COMMON_TRANSLATION("label", __VA_ARGS__)

namespace base::ui::localization {
  const translation_map_t default_translation = {
    ADD_TRANSLATION("text/hello_world", "Hello World!"),
    ADD_TRANSLATION(LABEL_TRANSLATION("ok"), "Ok"),
    ADD_TRANSLATION(LABEL_TRANSLATION("cancel"), "Cancel"),
    ADD_TRANSLATION(LABEL_TRANSLATION("yes"), "Yes"),
    ADD_TRANSLATION(LABEL_TRANSLATION("No"), "No")
  };
}
