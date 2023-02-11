//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
#define GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
#include <cstdint>
#include <string>
#include "../types.hpp"
#include "../../../misc/spinlock.hpp"

namespace gta_base::ui::draw::components {
  class Submenu {
  public:
    explicit Submenu(sub_id_t parent_sub, sub_id_t sub_id) : parent_(parent_sub), sub_id_(sub_id) {}

    [[nodiscard]] inline sub_id_t GetParentId() const {
      return parent_;
    }

    [[nodiscard]] inline bool IsRoot() const {
      return parent_ < 0;
    }

    [[nodiscard]] inline sub_id_t GetSubId() const {
      return sub_id_;
    }

    [[nodiscard]] std::string GetName();

    [[nodiscard]] inline std::string GetNameRaw() {
      return name_;
    }

    inline void SetName(const std::string& name) {
      name_ = name;
    }

    [[nodiscard]] inline std::size_t GetSelectedOptionIdx() const {
      return selected_option_idx_;
    }

    inline void SetSelectedOptionIdx(std::size_t idx) {
      selected_option_idx_ = idx;
    }

  private:
    misc::RecursiveSpinlock lock_;
    const sub_id_t parent_;
    const sub_id_t sub_id_;
    std::string name_;
    std::size_t selected_option_idx_{};

  private:

  };
}
#endif //GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
