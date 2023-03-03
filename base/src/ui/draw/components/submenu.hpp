//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
#define GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
#include <cstdint>
#include <string>
#include <utility>
#include "../types.hpp"
#include "../../../misc/spinlock.hpp"
#include "../../../key_input/manager.hpp"
#include "options/base_option.hpp"

namespace gta_base::ui::draw::components {
  class Submenu {
  public:
    using create_options_cb_t = std::function<void(Submenu*)>;

  public:
    explicit Submenu(sub_id_t sub_id, std::string name, create_options_cb_t create_options_cb) : sub_id_(sub_id), name_(std::move(name)), create_options_cb_(std::move(create_options_cb)) {}

    [[nodiscard]] inline bool IsRoot() const {
      return sub_id_ < 0;
    }

    [[nodiscard]] inline sub_id_t GetSubId() const {
      return sub_id_;
    }

    [[nodiscard]] inline std::string GetName() {
      return kTRANSLATION_MANAGER->Get(name_);
    }

    [[nodiscard]] inline std::string GetNameRaw() {
      misc::ScopedSpinlock lock(lock_);
      return name_;
    }

    inline void SetName(const std::string& name) {
      misc::ScopedSpinlock lock(lock_);
      name_ = name;
    }

    template<typename T>
    requires std::derived_from<T, BaseOption>
    inline BaseOption::option_ptr AddOption(T option) {
      misc::ScopedSpinlock lock(lock_);
      options_.emplace_back(std::make_shared<T>(std::forward<T>(option)));

      return options_.back();
    }

    [[nodiscard]] inline BaseOption::option_ptr GetSelectedOption() {
      misc::ScopedSpinlock lock(lock_);
      return options_[selected_option_idx_];
    }

    [[nodiscard]] inline std::size_t GetSelectedOptionIdx() {
      misc::ScopedSpinlock lock(lock_);
      return selected_option_idx_;
    }

    inline void SetSelectedOptionIdx(std::size_t idx) {
      misc::ScopedSpinlock lock(lock_);
      selected_option_idx_ = idx;
    }

    inline void ScrollUp() {
      misc::ScopedSpinlock lock(lock_);
      if (selected_option_idx_ == options_.size() - 1) {
        selected_option_idx_ = 0;
        return;
      }

      selected_option_idx_ += 1;
    }

    inline void ScrollDown() {
      misc::ScopedSpinlock lock(lock_);
      if (selected_option_idx_ == 0) {
        selected_option_idx_ = options_.size() - 1;
        return;
      }

      selected_option_idx_ -= 1;
    }

    void IterateOptions(const std::function<void(std::shared_ptr<BaseOption>)>& cb) {
      misc::ScopedSpinlock lock(lock_);
      for (auto&& opt : options_)
        cb(opt);
    }

  private:
    misc::RecursiveSpinlock lock_;
    const sub_id_t sub_id_;
    std::string name_;
    create_options_cb_t create_options_cb_;
    std::size_t selected_option_idx_{};
    std::vector<std::shared_ptr<BaseOption>> options_;
  };
}
#endif //GTA_BASE_SUBMENU_3E79EEE6B1D349D0BF83D7F4C3D94426_HPP
