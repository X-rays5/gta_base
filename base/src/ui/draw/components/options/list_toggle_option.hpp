//
// Created by X-ray on 02/23/23.
//

#pragma once
#ifndef GTA_BASE_LIST_TOGGLE_OPTION_2BD67D9AC4754974A9DFB033233EE964_HPP
#define GTA_BASE_LIST_TOGGLE_OPTION_2BD67D9AC4754974A9DFB033233EE964_HPP
#include "list_option.hpp"
#include "bool_option.hpp"

namespace gta_base::ui::draw::components {
  template<typename arr_t, typename access_t>
  class ListToggleOption final : public ListOption<arr_t, access_t>, public BoolOption {
  public:
    ListToggleOption(std::string name, std::string description, bool& toggle, arr_t& array, access_t& arr_idx, flags::OptionFlags flags = flags::OptionFlags::kNone) :
    ListOption<arr_t, access_t>(name, description, array, arr_idx),
    BoolOption(name, description, toggle) {}

    std::string Serialize() override {
      return fmt::format("{}##{}", this->arr_idx_, this->toggle_ ? "true" : "false");
    }

    void Deserialize(const std::string& val) override {
      if (auto pos = val.find("##"); pos != std::string::npos) {
        this->toggle_ = val.substr(pos + 2) == "true";
        this->arr_idx_ = common::LexicalCast<access_t>(val);
      }
      if (this->arr_idx_ < 0)
        this->arr_idx_ = 0;
      else if (this->arr_idx_ >= this->arr_size_)
        this->arr_idx_ = this->arr_size_ - 1;
    }

    void HandleKey(key_input::KeyBinds key_bind) override {
      switch (key_bind) {
        case key_input::KeyBinds::ui_select: {
          this->toggle_ = !this->toggle_;
          this->SendEvent(OptionEvent::kChange);
          break;
        }
        case key_input::KeyBinds::ui_left: {
          this->ScrollLeft();
          this->SendEvent(OptionEvent::kChange);
          break;
        }
        case key_input::KeyBinds::ui_right: {
          this->ScrollRight();
          this->SendEvent(OptionEvent::kChange);
          break;
        }
        default:
          break;
      }
    }
  };
}
#endif //GTA_BASE_LIST_TOGGLE_OPTION_2BD67D9AC4754974A9DFB033233EE964_HPP
