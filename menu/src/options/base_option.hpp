//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "base_command.hpp"
#include "../game/globals.hpp"
#include "../game/local_player.hpp"
#include "../script/game_task_executor.hpp"
#include "../ui/components/components.hpp"

namespace base::menu::options {
#define GTA_BASE_OPTION_UI_COMP(sub, comp) sub->AddComponent(this, comp)
#define GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label) sub->AddComponent(this, ui::components::ExecuteComponent(label, GetDescriptionRaw(), this))

  class BaseOption : protected BaseCommand {
  public:
    enum class ThreadType {
      kNONE,
      kGAME_SCRIPT
    };

    explicit BaseOption(const std::string& name) : BaseCommand(name, "") {}
    BaseOption(const std::string& name, const std::string& description) : BaseCommand(name, description) {}
    virtual ~BaseOption() override = default;

    virtual std::string GetName() const override {
      return BaseCommand::GetName();
    }

    virtual std::string GetDescription() const override {
      return BaseCommand::GetDescription();
    }

    virtual std::string GetDescriptionRaw() const override {
      return BaseCommand::GetDescription();
    }

    virtual std::string GetHelp() const override {
      return BaseCommand::GetHelp();
    }

    /// This option as the command it is. Inheriting BaseCommand protected keeps the command surface -
    /// GetArgs, GetParser - out of an option's callers, since what is called through an option is the
    /// option; parsing a command line for one is the case that does need it, and this is how that case
    /// gets there. The reference is to this same object, so execute() on it is still the override above.
    BaseCommand& GetCommand() {
      return *this;
    }

    const BaseCommand& GetCommand() const {
      return *this;
    }

    /// The script this option belongs to, or empty for one the menu itself registered. What the
    /// registry asks before letting go of an option that a script registered, so that unloading the
    /// script takes its options out with it.
    virtual std::string GetOwnerScript() const {
      return {};
    }

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) override final {
      DispatchToOptionThread([this] {
        runCommand(std::make_shared<argparse::ArgumentParser>());
      });
    }

    virtual bool IsHotkeyAble() const {
      return false;
    }

    void TriggerHotkey() {
      if (!IsHotkeyAble()) {
        return;
      }

      // A key is pressed on the thread that watches for it, which is not the thread an option is meant
      // to run on, so a hotkey is dispatched exactly as a command is - the same thread, whichever way the
      // option was started. That is what makes the handlers callable at all: they are native calls like
      // any other, and the game is only safe to touch from its own thread.
      DispatchToOptionThread([this] {
        if (!HandleHotkey()) {
          LOG_WARN("Hotkey triggered for option {} but it does not have a hotkey handler", GetName());
        }
      });
    }

    virtual bool IsSavable() const {
      return false;
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

    virtual ThreadType GetThreadType() const {
      return ThreadType::kGAME_SCRIPT;
    }

    void operator()() {
      execute(std::make_shared<argparse::ArgumentParser>());
    }

  protected:
    /// Runs an option's work on the thread it asks for: the game's script thread when the work is native
    /// calls - which is what GetThreadType answers, and the default answer - and the pool otherwise.
    /// Every entry into an option comes through here, so which thread it runs on is a property of the
    /// option and not of how it was reached.
    template <typename F>
    void DispatchToOptionThread(F&& body) {
      switch (GetThreadType()) {
      case ThreadType::kNONE: {
        if (!util::kTHREAD_POOL) {
          LOG_ERROR("ThreadPool is not initialized, cannot execute option {} on thread pool", GetName());
          return;
        }

        util::kTHREAD_POOL->emplace_back(std::forward<F>(body));
      }
      break;
      case ThreadType::kGAME_SCRIPT: {
        if (!script::kGAME_TASK_EXECUTOR) {
          LOG_ERROR("GameTaskExecutor is not initialized, cannot execute option {} on game thread", GetName());
          return;
        }

        script::kGAME_TASK_EXECUTOR->QueueTask(std::forward<F>(body));
      }
      }
    }

    virtual bool HandleHotkey() {
      if (IsHotkeyAble()) {
        return false;
      }

      return true;
    }

    virtual void runCommand(std::shared_ptr<argparse::ArgumentParser> args) = 0;
  };
}