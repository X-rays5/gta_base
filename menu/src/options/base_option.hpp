//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "base_command.hpp"
#include "../ui/components/components.hpp"

namespace base::menu::options {
#define GTA_BASE_OPTION_UI_COMP(sub, comp) sub->AddComponent(this, comp)

  class BaseOption : public BaseCommand {
  public:
    enum class TickThread {
      kNONE,
      kGAME_SCRIPT
    };

    public:
    explicit BaseOption(const std::string& name) : BaseCommand(name, "") {}
    BaseOption(const std::string& name, const std::string& description) : BaseCommand(name, description) {}
    ~BaseOption() override = default;

    virtual bool IsHotkeyAble() const {
      return false;
    }

    virtual bool IsSavable() const {
      return false;
    }

    virtual void HandleHotkey() {
      if (IsHotkeyAble()) {
        LOG_ERROR("HandleHotkey called on option {} which is does not have a hotkey handler", GetName());
      }
    }

    virtual void Save(glz::generic&) {
      if (IsSavable()) {
        LOG_ERROR("Save called on option {} which is not savable", GetName());
      }
    }

    virtual void Load(const glz::generic&) {
      if (IsSavable()) {
        LOG_ERROR("Load called on option {} which is not savable", GetName());
      }
    }

    virtual void CreateOptionUi(const std::string& label, ui::Submenu* sub) = 0;

    virtual void Tick() {}

    virtual bool IsTickable() const {
      return false;
    }

    virtual TickThread GetTickThread() const {
      return TickThread::kNONE;
    }
  };
}