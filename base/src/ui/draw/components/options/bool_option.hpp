//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_BOOL_OPTION_E97141F56A064A6FA568815E2BC53ED8_HPP
#define GTA_BASE_BOOL_OPTION_E97141F56A064A6FA568815E2BC53ED8_HPP
#include <utility>

#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  class BoolOption : public virtual BaseOption {
  public:
    BoolOption(const std::string& name, const std::string& description, bool& toggle, flags::OptionFlags flags = flags::OptionFlags::kHotkeyAble | flags::OptionFlags::kSaveAble) : toggle_(toggle),
    BaseOption(name, description, nullptr, flags | flags::OptionFlags::kToggle) {}

    std::string Serialize() override {
      return toggle_ ? "true" : "false";
    }

    void Deserialize(const std::string& val) override {
      toggle_ = val == "true";
    }

    void HandleKey(key_input::KeyBinds key_bind) override {
      if (key_bind == key_input::KeyBinds::ui_select) {
        toggle_ = !toggle_;
        this->SendEvent(OptionEvent::kChange);
      }
    }

    [[nodiscard]] bool GetState() const {
      return toggle_;
    }

  private:
    bool& toggle_;
  };
}
#endif //GTA_BASE_BOOL_OPTION_E97141F56A064A6FA568815E2BC53ED8_HPP
