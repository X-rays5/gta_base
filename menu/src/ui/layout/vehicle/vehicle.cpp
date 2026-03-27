//
// Created by X-ray on 27/03/2026.
//

#include "vehicle.hpp"
#include "../../../options/option_registry.hpp"
#include "../../components/components.hpp"

namespace base::menu::ui::layout {
  void InitVehicleLayout() {

    Submenu vehicle_submenu("ui/sub/vehicle", [](Submenu* sub) {
      if (options::kOPTION_REGISTRY) {
        const auto options = options::kOPTION_REGISTRY->options().lock();

        options->vehicle_repair_option->CreateOptionUi("ui/action/vehicle_repair", sub);
      }
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kVEHICLE, std::move(vehicle_submenu));
  }
}