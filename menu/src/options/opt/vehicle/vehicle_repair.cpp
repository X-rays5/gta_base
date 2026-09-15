//
// Created by X-ray on 27/03/2026.
//

#include "vehicle_repair.hpp"
#include "../../../game/globals.hpp"
#include "../../../natives/natives_gen9.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void RepairVehicle() {
      script::kGAME_TASK_EXECUTOR->QueueTask([] {
        const auto vehicle = game::globals::local_player.vehicle_id.load();
        if (vehicle.IsValid()) {
          vehicle.Fix();
        }
      });
    }
  }

  void VehicleRepairOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    RepairVehicle();
  }

  void VehicleRepairOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_UI_COMP(sub, ui::components::ExecuteComponent(label, GetDescriptionRaw(), RepairVehicle));
  }

  bool VehicleRepairOption::IsHotkeyAble() const {
    return true;
  }

  void VehicleRepairOption::HandleHotkey() {
    RepairVehicle();
  }
}

