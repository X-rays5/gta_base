//
// Created by X-ray on 17/09/2026.
//

#include "teleport_into_last_vehicle.hpp"

namespace base::menu::options {
  namespace {
    void IntoLastVehicle() {
      const auto local_player = game::LocalPlayer::Get();
      if (const auto last_vehicle_result = local_player.GetLastVehicle(); last_vehicle_result.has_value()) {
        if (const auto last_vehicle = last_vehicle_result.value(); last_vehicle.Exists()) {
          if (local_player.GetPed().IntoVehicle(last_vehicle))
            return;
        }
      }

      NOTIFY_ERR("label/teleport", "label/teleport/no_last_vehicle");
    }
  }

  void TeleportIntoLastVehicle::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    IntoLastVehicle();
  }

  bool TeleportIntoLastVehicle::IsHotkeyAble() const {
    return true;
  }

  bool TeleportIntoLastVehicle::HandleHotkey() {
    IntoLastVehicle();
    return true;
  }

  void TeleportIntoLastVehicle::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }
}