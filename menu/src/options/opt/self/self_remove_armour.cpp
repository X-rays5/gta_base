//
// Created by X-ray on 27/03/2026.
//

#include "self_remove_armour.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void RemoveArmour() {
      script::kGAME_TASK_EXECUTOR->QueueTask([] {
        natives::PED::SET_PED_ARMOUR(natives::PLAYER::PLAYER_PED_ID(), 0);
      });
    }
  }

  void SelfRemoveArmourOption::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    RemoveArmour();
  }

  void SelfRemoveArmourOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }

  bool SelfRemoveArmourOption::IsHotkeyAble() const {
    return true;
  }

  bool SelfRemoveArmourOption::HandleHotkey() {
    RemoveArmour();
    return true;
  }
}
