//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "opt/godmode.hpp"

namespace base::menu::options {
  struct AvailableOptions {
    AvailableOptions();

    std::shared_ptr<GodModeOption> god_mode_option;
  };
}