//
// Created by X-ray on 27/03/2026.
//

#include "self_remove_armour.hpp"
#include "../../../script/game_task_executor.hpp"

namespace base::menu::options {
  namespace {
    void RemoveArmour() {
      script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
        natives::PED::SET_PED_ARMOUR(natives::PLAYER::PLAYER_PED_ID(), 0);
      });
    }
  }

  void SelfRemoveArmourOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    RemoveArmour();
  }

  void SelfRemoveArmourOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    sub->AddComponent(ui::components::ExecuteComponent(label, GetDescriptionRaw(), RemoveArmour));
  }

  bool SelfRemoveArmourOption::IsHotkeyAble() const {
    return true;
  }

  void SelfRemoveArmourOption::HandleHotkey() {
    RemoveArmour();
  }
}
