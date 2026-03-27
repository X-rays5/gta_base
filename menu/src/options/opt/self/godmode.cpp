//
// Created by X-ray on 27/03/2026.
//

#include "godmode.hpp"
#include "../../../natives/natives_gen9.hpp"

namespace base::menu::options {
  void GodModeOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    sub->AddComponent(ui::components::ToggleComponent(label, GetDescriptionRaw(), &enabled_));
  }

  void GodModeOption::execute(std::shared_ptr<argparse::ArgumentParser>) {
    enabled_ = !enabled_;
  }

  void GodModeOption::HandleHotkey() {
    enabled_ = !enabled_;
  }

  bool GodModeOption::IsHotkeyAble() const {
    return true;
  }

  bool GodModeOption::IsSavable() const {
    return true;
  }

  void GodModeOption::Tick() {
    natives::ENTITY::SET_ENTITY_INVINCIBLE(natives::PLAYER::PLAYER_PED_ID(), true, false);
  }

  bool GodModeOption::IsTickable() const {
    return true;
  }

  BaseOption::TickThread GodModeOption::GetTickThread() const {
    return TickThread::kGAME_SCRIPT;
  }

  void GodModeOption::Save(glz::generic& data) {
    data["enabled"] = enabled_;
  }

  void GodModeOption::Load(const glz::generic& data) {
    if (data.contains("enabled") && data["enabled"].is_boolean()) {
      enabled_ = data["enabled"].get<bool>();
    } else {
      LOG_ERROR("Invalid data for godmode option state: {}", data);
    }
  }
}
