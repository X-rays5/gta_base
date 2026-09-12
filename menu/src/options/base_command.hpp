//
// Created by X-ray on 27/03/2026.
//

#pragma once

#include <argparse/argparse.hpp>
#include <string>
#include <vector>

#include "command_line.hpp"
#include "../ui/localization/manager.hpp"

namespace base::menu::options {
  /// One argument a command takes, which is where its parser comes from and where its help text comes
  /// from - both are written from the same declaration, so neither can describe something the other
  /// does not accept.
  struct CommandArg {
    /// The name the parser is given: "--level".
    std::string name;

    /// How the value is read, one of the type chars of command_line.hpp - 'i', 'u', 'f', 'd', 's' or
    /// 'b'. IsValidArgType is the check for one a script declares.
    char type{};

    /// What the argument is for, shown beside it in GetHelp() and read by nothing else.
    std::string help;
  };

  class BaseCommand {
  public:
    explicit BaseCommand(const std::string& name, const std::string& description) : name_(name), description_(description) {}
    virtual ~BaseCommand() = default;

    /// The parser of this command: the arguments it declares, and then whatever ModifyParser adds to
    /// them.
    std::shared_ptr<argparse::ArgumentParser> GetParser() const {
      auto parser = BuildParser(*this);
      ModifyParser(parser);
      return parser;
    }

    /// The usage text of this command: its name, its description, and one line per declared argument
    /// saying what the argument is and what it is for.
    ///
    /// A command that declares nothing, or whose arguments are added inside ModifyParser, has only its
    /// name and description here: a parser cannot be read back for a declaration, so GetArgs() is the
    /// only thing this can describe.
    std::string GetHelp() const {
      return FormatHelp(*this);
    }

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) = 0;

    virtual std::string GetName() const {
      return name_;
    }

    std::string GetDescription() const {
      // A description is a localization key, so without the localization manager the key itself is the
      // best answer there is - and the alternative is reading through a null pointer. A command
      // asked for its description outside the menu, as a test and a script callback both do, is
      // ordinary rather than a fault.
      if (!ui::localization::kMANAGER) {
        return description_;
      }

      return ui::localization::kMANAGER->Localize(description_);
    }

    std::string GetDescriptionRaw() const {
      return description_;
    }

    /// The arguments this command takes. A command that declares none takes none, so its parser
    /// accepts an empty command line and refuses anything else.
    virtual std::vector<CommandArg> GetArgs() const {
      return {};
    }

  protected:
    /// The escape hatch for a parser the declared arguments cannot describe - subparsers, variadics,
    /// mutually exclusive groups. An argument added here is invisible to GetHelp() and to the
    /// getters a script reads its callback's arguments with, so a command that wants an argument
    /// iterable or readable belongs in GetArgs() instead.
    virtual void ModifyParser(std::shared_ptr<argparse::ArgumentParser>) const {}

  private:
    const std::string name_;
    const std::string description_;
  };
}
