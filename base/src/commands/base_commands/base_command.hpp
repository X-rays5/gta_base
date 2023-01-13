//
// Created by X-ray on 01/08/23.
//

#pragma once
#ifndef GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
#define GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
#include <string>
#include "../../rage/joaat.hpp"
#include "../../ui/translation/translation_manager.hpp"
#include "../../ui/notification/notification.hpp"

namespace gta_base::commands::base_commands {
  class Base {
  public:
    Base(const std::string& id, const std::string& name_key, const std::string& description_key, std::size_t num_args = 0, bool fiber_pool = false);

    [[nodiscard]] FORCE_INLINE rage::joaat_t GetId() const {
      return id_;
    }

    [[nodiscard]] FORCE_INLINE std::string GetName() {
      return ui::kTRANSLATION_MANAGER->Get(name_key_);
    }

    [[nodiscard]] FORCE_INLINE std::string GetNameKey() {
      return name_key_;
    }

    [[nodiscard]] FORCE_INLINE std::string GetDescription() {
      return ui::kTRANSLATION_MANAGER->Get(description_key_);
    }

    [[nodiscard]] FORCE_INLINE std::string GetDescriptionKey() {
      return description_key_;
    }

    FORCE_INLINE void Run(const std::string& args) {
      if ((args.empty() || args == " ") && num_args_ == 0) {
        Execute({});
        return;
      }

      auto args_vec = common::SplitStr(args, " ");
      if (args_vec.size() < num_args_) {
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Command", "Not enough arguments");
        return;
      }

      if (args_vec.size() > num_args_) {
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Command", "Too many arguments");
        return;
      }

      Execute(args_vec);
    }

  protected:
    rage::joaat_t id_;
    std::string name_key_;
    std::string description_key_;
    std::size_t num_args_{};
    bool fiber_pool_;

  protected:
    virtual void Execute(std::vector<std::string> args) = 0;
  };
}
#endif //GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
