//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "base_command.hpp"
#include "../ui/components/components.hpp"

namespace base::menu::options {
  class BaseOption : public BaseCommand {
  public:
    enum class TickThread {
      kGAME_SCRIPT
    };

    public:
    explicit BaseOption(const std::string& name) : BaseCommand(name, "") {}
    BaseOption(const std::string& name, const std::string& description) : BaseCommand(name, description) {}
    ~BaseOption() override = default;

    virtual bool IsHotkeyAble() const = 0;
    virtual bool IsSavable() const = 0;

    virtual void HandleHotkey() = 0;
    virtual void Save(glz::generic& data) = 0;
    virtual void Load(const glz::generic& data) = 0;

    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) = 0;

    virtual void Tick() {}
    virtual bool IsTickable() const = 0;
    virtual TickThread GetTickThread() const = 0;
  };
}