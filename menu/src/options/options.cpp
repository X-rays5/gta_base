//
// Created by X-ray on 27/03/2026.
//

#include "options.hpp"
#include "option_registry.hpp"

namespace base::menu::options {
#define REGISTER_OPTION(field) field = std::make_shared<std::remove_reference_t<decltype(*field)>>(); \
  if (kOPTION_REGISTRY) { kOPTION_REGISTRY->AddOption(field); }

  AvailableOptions::AvailableOptions() {
    REGISTER_OPTION(god_mode_option);
  }
}
