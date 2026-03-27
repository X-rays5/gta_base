//
// Created by X-ray on 27/03/2026.
//

#pragma once

#include <argparse/argparse.hpp>
#include "../ui/localization/manager.hpp"

namespace base::menu::options {
  class BaseCommand {
  public:
    explicit BaseCommand(const std::string& name, const std::string& description) : name_(name), description_(description) {}
    virtual ~BaseCommand() = default;

    std::shared_ptr<argparse::ArgumentParser> GetParser() const {
      auto parser = std::make_shared<argparse::ArgumentParser>(GetName());
      const auto desc = GetDescription();
      if (!desc.empty()) {
        parser->add_description(desc);
      }

      ModifyParser(parser);
      return parser;
    }

    virtual void execute(std::shared_ptr<argparse::ArgumentParser> args) = 0;

    virtual std::string GetName() const {
      return name_;
    }

    std::string GetDescription() const {
      return ui::localization::kMANAGER->Localize(description_);
    }

    std::string GetDescriptionRaw() const {
      return description_;
    }

  protected:
    virtual void ModifyParser(std::shared_ptr<argparse::ArgumentParser>) const {}

  private:
    const std::string name_;
    const std::string description_;
  };
}