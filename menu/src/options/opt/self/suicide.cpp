//
// Created by X-ray on 27/03/2026.
//

#include "suicide.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void Suicide() {
      script::kGAME_TASK_EXECUTOR->QueueTask([] {
        natives::ENTITY::SET_ENTITY_HEALTH(natives::PLAYER::PLAYER_PED_ID(), 0, natives::PLAYER::PLAYER_PED_ID(), NULL);
      });
    }
  }

  void SuicideOption::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    Suicide();
  }

  void SuicideOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }

  bool SuicideOption::IsHotkeyAble() const {
    return true;
  }

  bool SuicideOption::HandleHotkey() {
    Suicide();
    return true;
  }
}
