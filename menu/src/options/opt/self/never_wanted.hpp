//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "../../base_option.hpp"

namespace base::menu::options {
  class NeverWantedOption final : public BaseOption {
  public:
    NeverWantedOption() : BaseOption("never_wanted") {}

    virtual bool IsHotkeyAble() const override;
    virtual bool IsSavable() const override;
    virtual void Save(glz::generic& data) override;
    virtual void Load(const glz::generic& data) override;
    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;
    virtual void Tick() override;
    virtual bool IsTickable() const override;

  protected:
    virtual void runCommand(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool HandleHotkey() override;

  private:
    std::atomic<bool> enabled_ = false;
  };
}

