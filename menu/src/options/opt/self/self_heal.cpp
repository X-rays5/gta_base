//
// Created by X-ray on 27/03/2026.
//

#include "self_heal.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void HealPlayer() {
      script::kGAME_TASK_EXECUTOR->QueueTask([] {
        natives::ENTITY::SET_ENTITY_HEALTH(natives::PLAYER::PLAYER_PED_ID(), natives::ENTITY::GET_ENTITY_MAX_HEALTH(natives::PLAYER::PLAYER_PED_ID()), natives::PLAYER::PLAYER_PED_ID(), NULL);
      });
    }
  }

  void SelfHealOption::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    HealPlayer();
  }

  bool SelfHealOption::IsHotkeyAble() const {
    return true;
  }

  bool SelfHealOption::HandleHotkey() {
    HealPlayer();
    return true;
  }

  void SelfHealOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }
}
