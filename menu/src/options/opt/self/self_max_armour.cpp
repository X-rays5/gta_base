//
// Created by X-ray on 27/03/2026.
//

#include "self_max_armour.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void MaxArmour() {
      script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
        natives::PED::SET_PED_ARMOUR(natives::PLAYER::PLAYER_PED_ID(), natives::PLAYER::GET_PLAYER_MAX_ARMOUR(natives::PLAYER::PLAYER_ID()));
      });
    }
  }

  void SelfMaxArmourOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    MaxArmour();
  }

  void SelfMaxArmourOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_UI_COMP(sub, ui::components::ExecuteComponent(label, GetDescriptionRaw(), MaxArmour));
  }

  bool SelfMaxArmourOption::IsHotkeyAble() const {
    return true;
  }

  void SelfMaxArmourOption::HandleHotkey() {
    MaxArmour();
  }
}
