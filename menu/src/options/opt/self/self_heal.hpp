//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "../../base_option.hpp"

namespace base::menu::options {
  class SelfHealOption final : public BaseOption {
  public:
    SelfHealOption() : BaseOption("selfheal") {}

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool IsHotkeyAble() const override;
    virtual void HandleHotkey() override;
    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;
  };
}
