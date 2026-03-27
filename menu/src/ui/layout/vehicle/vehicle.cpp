//
// Created by X-ray on 27/03/2026.
//

#include "vehicle.hpp"
#include "../../components/components.hpp"

namespace base::menu::ui::layout {
  void InitVehicleLayout() {

    Submenu vehicle_submenu("ui/sub/vehicle", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("ui/action/vehicle_repair", [] {
        script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
          natives::VEHICLE::SET_VEHICLE_FIXED(natives::PED::GET_VEHICLE_PED_IS_IN(natives::PLAYER::PLAYER_PED_ID(), false));
        });
      }));
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kVEHICLE, std::move(vehicle_submenu));
  }
}