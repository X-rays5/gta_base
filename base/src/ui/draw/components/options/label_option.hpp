//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_LABEL_OPTION_F5C392F3A06B4C3BBE9D59FFC3E19327_HPP
#define GTA_BASE_LABEL_OPTION_F5C392F3A06B4C3BBE9D59FFC3E19327_HPP
#include <utility>
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  class LabelOption final : public BaseOption {
  public:
    explicit LabelOption(std::string name) : BaseOption(std::move(name), {}, nullptr, flags::OptionFlags::kCenterText | flags::OptionFlags::kCantSelect) {}

    std::string Serialize() override {};
    void Deserialize(const std::string& val) override {};
    void HandleKey(key_input::KeyBinds key_bind) override {};
  };
}
#endif //GTA_BASE_LABEL_OPTION_F5C392F3A06B4C3BBE9D59FFC3E19327_HPP
