//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "../../base_option.hpp"

namespace base::menu::options {
  class ForceWantedLevelOption final : public BaseOption {
  public:
    ForceWantedLevelOption() : BaseOption("force_wanted_level", "ui/option/force_wanted_level/desc") {}

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool IsHotkeyAble() const override;
    virtual bool IsSavable() const override;
    virtual void HandleHotkey() override;
    virtual void Save(glz::generic& data) override;
    virtual void Load(const glz::generic& data) override;
    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;
    virtual void Tick() override;
    virtual bool IsTickable() const override;
    virtual TickThread GetTickThread() const override;

  private:
    std::atomic<bool> enabled_ = false;
    std::atomic<std::uint8_t> level_ = 0;
  };
}

