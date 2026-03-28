//
// Created by X-ray on 27/03/2026.
//

#include "never_wanted.hpp"

namespace base::menu::options {
  void NeverWantedOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    enabled_ = !enabled_;
  }

  bool NeverWantedOption::IsHotkeyAble() const {
    return true;
  }

  bool NeverWantedOption::IsSavable() const {
    return true;
  }

  void NeverWantedOption::HandleHotkey() {
    enabled_ = !enabled_;
  }

  void NeverWantedOption::Save(glz::generic& data) {
    data["enabled"] = enabled_;
  }

  void NeverWantedOption::Load(const glz::generic& data) {
    if (data.contains("enabled") && data["enabled"].is_boolean()) {
      enabled_ = data["enabled"].get<bool>();
    } else {
      LOG_ERROR("Invalid data for never_wanted option state: {}", data);
    }
  }

  void NeverWantedOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_UI_COMP(sub, ui::components::ToggleComponent(label, GetDescriptionRaw(), &enabled_));
  }

  void NeverWantedOption::Tick() {
    if (enabled_) {
      natives::PLAYER::CLEAR_PLAYER_WANTED_LEVEL(natives::PLAYER::PLAYER_ID());
      natives::PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(natives::PLAYER::PLAYER_ID(), false);
    }
  }

  bool NeverWantedOption::IsTickable() const {
    return true;
  }

  BaseOption::TickThread NeverWantedOption::GetTickThread() const {
    return TickThread::kGAME_SCRIPT;
  }
}

