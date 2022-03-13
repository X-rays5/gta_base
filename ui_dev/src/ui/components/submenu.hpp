//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_SUBMENU_HPP
#define GTA_BASE_SUBMENU_HPP
#include <utility>
#include <vector>
#include "option/baseoption.hpp"
#include "enums.hpp"

namespace ui {
  namespace components {
    class Submenu {
    public:
      using constructor_cb = void(*)(Submenu *);

      explicit Submenu(std::string name, Submenus submenu_id, constructor_cb cb) : name_(std::move(name)), submenu_id_(submenu_id) {
        cb(this);
      }

      [[nodiscard]] std::string GetName() const {
        return name_;
      };

      void SetName(std::string name) {
        name_ = std::move(name);
      };

      std::shared_ptr<option::BaseOption> GetOption(size_t index) {
        return options_[index];
      };

      size_t GetOptionCount() {
        return options_.size();
      };

      [[nodiscard]] size_t GetSelectedOption() const {
        return selected_option_;
      };

      void SetSelectedOption(std::int64_t index) {
        if (index >= options_.size())
          index = 0;
        if (index < 0)
          index = static_cast<std::int64_t>(options_.size()) - 1;
        selected_option_ = index;
      };

      template<typename T>
      void AddOption(T option) {
        options_.push_back(std::make_shared<T>(std::forward<T>(option)));
      }

      void HandleKey(KeyInput key) {
        if (key == KeyInput::kUp) {
          if (selected_option_ > 0) {
            SetSelectedOption(selected_option_ - 1);
          } else {
            SetSelectedOption(options_.size() - 1);
          }
        } else if (key == KeyInput::kDown) {
          if (selected_option_ < options_.size() - 1) {
            SetSelectedOption(selected_option_ + 1);
          } else {
            SetSelectedOption(0);
          }
        } else {
          GetOption(GetSelectedOption())->HandleKey(key);
        }
      };

    private:
      std::string name_{};
      std::vector<std::shared_ptr<option::BaseOption>> options_;
      std::int64_t selected_option_ = 0;
      Submenus submenu_id_;
    };
  }
}
#endif //GTA_BASE_SUBMENU_HPP
