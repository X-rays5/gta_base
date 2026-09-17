//
// Created by X-ray on 17/09/2026.
//

#include "teleport_to_objective.hpp"

namespace base::menu::options {
  void TeleportToObjective::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    game::LocalPlayer::Get().TeleportToObjective();
  }

  bool TeleportToObjective::IsHotkeyAble() const {
    return true;
  }

  bool TeleportToObjective::HandleHotkey() {
    game::LocalPlayer::Get().TeleportToObjective();
    return true;
  }

  void TeleportToObjective::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }
}
