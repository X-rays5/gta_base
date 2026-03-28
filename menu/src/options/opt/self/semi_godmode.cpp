//
// Created by X-ray on 27/03/2026.
//

#include "semi_godmode.hpp"

namespace base::menu::options {
  void SemiGodModeOpt::execute(std::shared_ptr<argparse::ArgumentParser>) {
    enabled_ = !enabled_;
  }

  bool SemiGodModeOpt::IsHotkeyAble() const {
    return true;
  }

  bool SemiGodModeOpt::IsSavable() const {
    return true;
  }

  void SemiGodModeOpt::HandleHotkey() {
    enabled_ = !enabled_;
  }

  void SemiGodModeOpt::Save(glz::generic& data) {
    data["enabled"] = enabled_;
  }

  void SemiGodModeOpt::Load(const glz::generic& data) {
    if (data.contains("enabled") && data["enabled"].is_boolean()) {
      enabled_ = data["enabled"].get<bool>();
    } else {
      LOG_ERROR("Invalid data for semigodmode option state: {}", data);
    }
  }

  void SemiGodModeOpt::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_UI_COMP(sub, ui::components::ToggleComponent(label, GetDescriptionRaw(), &enabled_));
  }

  bool SemiGodModeOpt::IsTickable() const {
    return true;
  }

  BaseOption::TickThread SemiGodModeOpt::GetTickThread() const {
    return TickThread::kGAME_SCRIPT;
  }

  void SemiGodModeOpt::Tick() {
    if (enabled_) {
      natives::ENTITY::SET_ENTITY_HEALTH(natives::PLAYER::PLAYER_PED_ID(), natives::ENTITY::GET_ENTITY_MAX_HEALTH(natives::PLAYER::PLAYER_PED_ID()), natives::PLAYER::PLAYER_PED_ID(), NULL);
    }
  }
}
