//
// Created by X-ray on 17/09/2026.
//

#pragma once
#include "../../../base_option.hpp"

namespace base::menu::options {
  class TeleportToObjective final : public BaseOption {
  public:
    TeleportToObjective() : BaseOption("teleport_to_objective", "") {}

    virtual bool IsHotkeyAble() const override;
    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;

  protected:
    virtual void runCommand(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool HandleHotkey() override;
  };
}