//
// Created by X-ray on 02/28/23.
//

#pragma once
#ifndef GTA_BASE_NUMBER_TOGGLE_OPTION_B23E805A23184D44941F856ED9D8CEF2_HPP
#define GTA_BASE_NUMBER_TOGGLE_OPTION_B23E805A23184D44941F856ED9D8CEF2_HPP
#include "number_option.hpp"

namespace gta_base::ui::draw::components {
  template<typename T>
  class NumberToggleOption final : public NumberOption<T> {
  public:
    NumberToggleOption(std::string name, std::string description, bool& toggle, T& val, T step, T min_val, T max_val, flags::OptionFlags flags = flags::OptionFlags::kNone) : toggle_(toggle),
    NumberOption<T>(std::move(name), std::move(description), std::move(val), std::move(step), std::move(min_val), std::move(max_val), flags) {}

    std::string Serialize() override {
      return fmt::format("{}##{}", this->val_, toggle_ ? "true" : "false");
    }

    void Deserialize(const std::string& val) override {
      if (auto pos = val.find("##"); pos != std::string::npos) {
        toggle_ = val.substr(pos + 2) == "true";
        this->val_ = common::LexicalCast<T>(val);
      }
      this->val_ = common::LexicalCast<T>(val);
      if (this->val_ < this->min_val_)
        this->val_ = this->min_val_;
      else if (this->val_ > this->max_val_)
        this->val_ = this->max_val_;
    }

    void HandleKey(key_input::KeyBinds key_bind) override {
      switch (key_bind) {
        case key_input::KeyBinds::ui_select: {
          toggle_ = !toggle_;
          this->SendEvent(OptionEvent::kSelect);
          break;
        }
        case key_input::KeyBinds::ui_left: {
          this->ScrollLeft();
          break;
        }
        case key_input::KeyBinds::ui_right: {
          this->ScrollRight();
          break;
        }
        default:
      }
    }

  private:
    bool& toggle_;
  };
}
#endif //GTA_BASE_NUMBER_TOGGLE_OPTION_B23E805A23184D44941F856ED9D8CEF2_HPP
