//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_DISABLED_OPTION_D5E8945363184B68BC48EFE21EC7A766_HPP
#define GTA_BASE_DISABLED_OPTION_D5E8945363184B68BC48EFE21EC7A766_HPP
#include "label_option.hpp"

namespace gta_base::ui::draw::components {
  class DisabledOption final : public BaseOption {
  public:
    explicit DisabledOption(std::string name) : BaseOption(std::move(name), {}, nullptr, flags::OptionFlags::kDarkText | flags::OptionFlags::kCantSelect) {}

    std::string Serialize() override {};
    void Deserialize(const std::string& val) override {};
    void HandleKey(key_input::KeyBinds key_bind) override {};
  };
}
#endif //GTA_BASE_DISABLED_OPTION_D5E8945363184B68BC48EFE21EC7A766_HPP
