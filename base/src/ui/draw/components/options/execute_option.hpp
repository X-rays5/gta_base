//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_EXECUTE_OPTION_FD5B4B5A8E474649A241A68BBAB9F0A3_HPP
#define GTA_BASE_EXECUTE_OPTION_FD5B4B5A8E474649A241A68BBAB9F0A3_HPP
#include <utility>
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  class ExecuteOption final : BaseOption {
  public:
    using action_t = std::function<void()>;

  public:
    ExecuteOption(std::string name, std::string description, action_t action = nullptr, flags::OptionFlags flags = flags::OptionFlags::kNone) : action_cb_(std::move(action)), BaseOption(std::move(name), std::move(description), nullptr, flags) {}

    std::string Serialize() override {};
    void Deserialize(const std::string& val) override {};

    void HandleKey(key_input::KeyBinds key_bind) override {
      if (key_input::KeyBinds::ui_select == key_bind && action_cb_)
        action_cb_();
    };

  private:
    action_t action_cb_;
  };
}
#endif //GTA_BASE_EXECUTE_OPTION_FD5B4B5A8E474649A241A68BBAB9F0A3_HPP
