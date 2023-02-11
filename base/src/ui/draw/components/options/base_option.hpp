//
// Created by X-ray on 02/11/23.
//

#pragma once
#ifndef GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
#define GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
#include "../../../../commands/command_manager.hpp"
#include "../../flags.hpp"

namespace gta_base::ui::draw::components {
  enum class OptionEvent {

  };

  class BaseOption {
  public:
    using event_cb_t = std::function<void(OptionEvent)>;

  public:
    BaseOption(const std::string& name, const std::string& description, event_cb_t cb_fn, flags::OptionFlags flags = flags::OptionFlags::kNone) : name_(name), description_(description), cb_fn_(std::move(cb_fn)), flags_(flags) {}
    [[nodiscard]] inline bool HasFlag(const flags::OptionFlags flag) {
      return flags_ & flag;
    }

    virtual std::string Serialize() = 0;
    virtual void Deserialize(const std::string& val) = 0;

  protected:
    misc::Flags<flags::OptionFlags> flags_;
    std::string name_;
    std::string description_;
    event_cb_t cb_fn_;

  protected:

  };
}
#endif //GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
