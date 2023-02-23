//
// Created by X-ray on 02/11/23.
//

#pragma once
#ifndef GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
#define GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
#include <utility>
#include "../../../../commands/command_manager.hpp"
#include "../../flags.hpp"
#include "../../types.hpp"
#include "../../../../key_input/manager.hpp"

namespace gta_base::ui::draw::components {
  enum class OptionEvent {
    kChange,
    kSelect,
  };

  class BaseOption {
  public:
    using event_cb_t = std::function<void(OptionEvent)>;
    using option_ptr = std::shared_ptr<BaseOption>;

  protected:
    BaseOption(std::string name, std::string description, event_cb_t cb_fn, flags::OptionFlags flags = flags::OptionFlags::kNone) : name_(std::move(name)), description_(std::move(description)), cb_fn_(std::move(cb_fn)), flags_(flags) {}

  public:
    [[nodiscard]] inline bool HasFlag(const flags::OptionFlags flag) {
      return flags_ & flag;
    }

    [[nodiscard]] inline std::string GetName() {
      return kTRANSLATION_MANAGER->Get(name_);
    }

    [[nodiscard]] inline std::string GetNameRaw() {
      return name_;
    }

    [[nodiscard]] inline std::string GetDescription() {
      return kTRANSLATION_MANAGER->Get(description_);
    }

    [[nodiscard]] inline std::string GetDescriptionRaw() {
      return description_;
    }

    [[nodiscard]] inline std::string GetRightText() const {
      return right_text_;
    }

    inline void SetEventHandler(event_cb_t cb_fn) {
      cb_fn_ = std::move(cb_fn);
    }

    virtual std::string Serialize() = 0;
    virtual void Deserialize(const std::string& val) = 0;

    virtual void HandleKey(key_input::KeyBinds key_bind) = 0;

  protected:
    flags::OptionFlags flags_;
    std::string name_;
    std::string description_;
    std::string right_text_;
    event_cb_t cb_fn_;

  protected:
    inline void SendEvent(OptionEvent event) {
      if (cb_fn_)
        std::invoke(cb_fn_, event);
    }
  };
}
#endif //GTA_BASE_BASE_OPTION_C5E6B495A8BE46BDBE36E0EFDAF44146_HPP
