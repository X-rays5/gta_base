//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "../../base_option.hpp"

namespace base::menu::options {
  class GodModeOption final : public BaseOption {
    public:
    GodModeOption() : BaseOption("godmode") {}

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) override;
    virtual bool IsHotkeyAble() const override;
    virtual bool IsSavable() const override;
    virtual void HandleHotkey() override;
    virtual void Save(glz::generic& data) override;
    virtual void Load(const glz::generic& data) override;
    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;
    virtual bool IsTickable() const override;
    virtual TickThread GetTickThread() const override;
    virtual void Tick() override;

  private:
    std::atomic<bool> enabled_ = false;
  };
}
