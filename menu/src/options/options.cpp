//
// Created by X-ray on 27/03/2026.
//

#include "options.hpp"
#include "option_registry.hpp"

namespace base::menu::options {
#define REGISTER_OPTION(field) field = std::make_shared<std::remove_reference_t<decltype(*field)>>(); \
  if (kOPTION_REGISTRY) { kOPTION_REGISTRY->AddOption(field); }

  AvailableOptions::AvailableOptions() {
    REGISTER_OPTION(god_mode_option)
    REGISTER_OPTION(semi_god_mode_option)
    REGISTER_OPTION(self_heal_option)
    REGISTER_OPTION(suicide_option)
    REGISTER_OPTION(self_max_armour_option)
    REGISTER_OPTION(self_remove_armour_option)
    REGISTER_OPTION(force_wanted_level_option)
    REGISTER_OPTION(never_wanted_option)
    REGISTER_OPTION(vehicle_repair_option)
  }
}
