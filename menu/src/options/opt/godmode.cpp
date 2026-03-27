//
// Created by X-ray on 27/03/2026.
//

#include "godmode.hpp"
#include "../../natives/natives_gen9.hpp"
#include <glaze/toml.hpp>

namespace base::menu::options {
  void GodModeOption::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    sub->AddComponent(ui::components::ToggleComponent(label, &enabled_));
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

  std::string GodModeOption::Save() {
    std::string res;
    glz::generic tmp;
    tmp["enabled"] = enabled_;

    const auto ec = glz::write_toml(tmp, res);
    LOG_ERROR_CONDITIONAL(ec, "Failed to save godmode option state to JSON: {}", ec);
    return res;
  }

  void GodModeOption::Load(const std::string& json) {
    glz::generic tmp;
    const auto ec = glz::read_toml(tmp, json);
    LOG_ERROR_CONDITIONAL(ec, "Failed to load godmode option state from JSON: {}, {}", json, ec);

    if (tmp.contains("enabled") && tmp["enabled"].is_boolean()) {
      enabled_ = tmp["enabled"].get<bool>();
    } else {
      LOG_ERROR("Invalid JSON for godmode option state: {}", json);
    }
  }
}
