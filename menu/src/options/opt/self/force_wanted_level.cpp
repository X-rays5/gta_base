//
// Created by X-ray on 27/03/2026.
//

#include "force_wanted_level.hpp"

namespace base::menu::options {
  void ForceWantedLevelOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    enabled_ = !enabled_;
  }

  bool ForceWantedLevelOption::IsHotkeyAble() const {
    return true;
  }

  bool ForceWantedLevelOption::IsSavable() const {
    return true;
  }

  void ForceWantedLevelOption::HandleHotkey() {
    enabled_ = !enabled_;
  }

  void ForceWantedLevelOption::Save(glz::generic& data) {
    data["enabled"] = enabled_;
    data["level"] = level_;
  }

  void ForceWantedLevelOption::Load(const glz::generic& data) {
    if (data.contains("enabled") && data["enabled"].is_boolean()) {
      enabled_ = data["enabled"].get<bool>();
    } else {
      LOG_ERROR("Invalid data for force_wanted_level option enabled: {}", data);
    }

    if (data.contains("level") && data["level"].is_number()) {
      level_ = static_cast<std::uint8_t>(data["level"].get<double>());
    } else {
      LOG_ERROR("Invalid data for force_wanted_level option level: {}", data);
    }
  }

  void ForceWantedLevelOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
   GTA_BASE_OPTION_UI_COMP(sub, ui::components::ToggleNumberRangeComponent<std::uint8_t>(label, GetDescriptionRaw(), &level_, 0, 5, 1, &enabled_));
  }

  void ForceWantedLevelOption::Tick() {
    if (enabled_) {
      natives::PLAYER::SET_PLAYER_WANTED_LEVEL(natives::PLAYER::PLAYER_ID(), level_, false);
      natives::PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(natives::PLAYER::PLAYER_ID(), false);
    }
  }

  bool ForceWantedLevelOption::IsTickable() const {
    return true;
  }

  BaseOption::TickThread ForceWantedLevelOption::GetTickThread() const {
    return TickThread::kGAME_SCRIPT;
  }
}

