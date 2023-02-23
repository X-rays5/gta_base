//
// Created by X-ray on 02/23/23.
//

#pragma once
#ifndef GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
#define GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  template<typename T> requires std::is_arithmetic_v<T>
  class NumberOption : public BaseOption {
  public:
    NumberOption(std::string name, std::string description, T& val, T step, T min_val, T max_val, flags::OptionFlags flags = flags::OptionFlags::kNone) : val_(val), step_(step), min_val_(min_val), max_val_(max_val),
    BaseOption(std::move(name), std::move(description), nullptr, flags::OptionFlags::kSaveAble | flags) {}

  protected:
    T& val_;
    T step_;
    T min_val_;
    T max_val_;
  };
}
#endif //GTA_BASE_NUMBER_OPTION_8D58148BC27A4009A7188A3684E2FCCC_HPP
