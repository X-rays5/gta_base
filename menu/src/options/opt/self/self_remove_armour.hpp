//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "../../base_option.hpp"

namespace base::menu::options {
  class SelfRemoveArmourOption : public BaseOption {
  public:
    SelfRemoveArmourOption() : BaseOption("self_remove_armour") {}

    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;
    virtual bool IsHotkeyAble() const override;

  protected:
    virtual void runCommand(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool HandleHotkey() override;
  };
}
