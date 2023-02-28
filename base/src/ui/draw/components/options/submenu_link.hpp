//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_SUBMENU_LINK_EAB89459D6154E488C4C1C0FE1D1FC41_HPP
#define GTA_BASE_SUBMENU_LINK_EAB89459D6154E488C4C1C0FE1D1FC41_HPP
#include <utility>
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  class SubmenuLink final : public BaseOption {
  public:
    SubmenuLink(sub_id_t link_to, std::string name, std::string description, BaseOption::event_cb_t event_cb = nullptr) : link_to_(link_to), BaseOption(std::move(name), std::move(description), std::move(event_cb), flags::OptionFlags::kSubmenuLink) {}

    std::string Serialize() override { return {}; }
    void Deserialize(const std::string& val) override {}

    void HandleKey(key_input::KeyBinds key_bind) override {}

    [[nodiscard]] inline sub_id_t GetLinkDestination() const {
      return link_to_;
    }

  private:
    sub_id_t link_to_;
  };
}
#endif //GTA_BASE_SUBMENU_LINK_EAB89459D6154E488C4C1C0FE1D1FC41_HPP
