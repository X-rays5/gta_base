//
// Created by X-ray on 27/03/2026.
//

#include "vehicle_repair.hpp"
#include "../../../script/game_task_executor.hpp"
#include "../../../natives/natives_gen9.hpp"

namespace base::menu::options {
  namespace {
    void RepairVehicle() {
      script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
        natives::VEHICLE::SET_VEHICLE_FIXED(natives::PED::GET_VEHICLE_PED_IS_IN(natives::PLAYER::PLAYER_PED_ID(), false));
      });
    }
  }

  void VehicleRepairOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    RepairVehicle();
  }

  void VehicleRepairOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    sub->AddComponent(ui::components::ExecuteComponent(label, GetDescriptionRaw(), RepairVehicle));
  }

  bool VehicleRepairOption::IsHotkeyAble() const {
    return true;
  }

  void VehicleRepairOption::HandleHotkey() {
    RepairVehicle();
  }
}

