//
// Created by X-ray on 17/09/2026.
//

#include "teleport_to_waypoint.hpp"

namespace base::menu::options {
  void TeleportToWaypoint::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    game::LocalPlayer::Get().TeleportToWaypoint();
  }

  bool TeleportToWaypoint::IsHotkeyAble() const {
    return true;
  }

  bool TeleportToWaypoint::HandleHotkey() {
    game::LocalPlayer::Get().TeleportToWaypoint();
    return true;
  }

  void TeleportToWaypoint::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_UI_COMP(sub, ui::components::ExecuteComponent(label, GetDescriptionRaw(), this));
  }
}
