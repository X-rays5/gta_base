//
// Created by X-ray on 02/23/23.
//

#pragma once
#ifndef GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
#define GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  template<typename T> requires std::is_arithmetic_v<T>
  class NumberOption : public virtual BaseOption {
  public:
    NumberOption(std::string name, std::string description, T& val, T step, T min_val, T max_val, flags::OptionFlags flags = flags::OptionFlags::kSaveAble) : val_(val), step_(step), min_val_(min_val), max_val_(max_val),
    BaseOption(std::move(name), std::move(description), nullptr, flags) {}

    std::string Serialize() override {
      return common::ToString(val_);
    }

    void Deserialize(const std::string& val) override {
      val_ = common::LexicalCast<T>(val);
      if (val_ < min_val_)
        val_ = min_val_;
      else if (val_ > max_val_)
        val_ = max_val_;
    }

    void HandleKey(key_input::KeyBinds key_bind) override {
      switch (key_bind) {
        case key_input::KeyBinds::ui_select: {
          SendEvent(OptionEvent::kSelect);
          break;
        }
        case key_input::KeyBinds::ui_left: {
          this->SendEvent(OptionEvent::kChange);
          break;
        }
        case key_input::KeyBinds::ui_right: {
          ScrollRight();
          this->SendEvent(OptionEvent::kChange);
          break;
        }
        default:
          break;
      }
    }

  protected:
    T& val_;
    T step_;
    T min_val_;
    T max_val_;

  protected:
    inline void ScrollLeft() {
      if (val_ >= step_)
        val_ -= step_;
      else
        val_ = max_val_ - (step_ - val_);
      if (val_ <= min_val_)
        val_ = max_val_;
    }

    inline void ScrollRight() {
      val_ += step_;
      if (val_ >= max_val_)
        val_ = min_val_;
    }
  };
}
#endif //GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
