//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_SUBMENU_HPP
#define GTA_BASE_SUBMENU_HPP
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "options/baseoption.hpp"
#include "enums.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      class Submenu {
      public:
        using constructor_cb = std::function<void(Submenu*)>;

        explicit Submenu(std::string name, Submenus submenu_id, constructor_cb cb) : name_(std::move(name)) {
          std::invoke(cb, this);
        }

        [[nodiscard]] inline std::string GetName() const {
          return name_;
        };

        inline void SetName(std::string name) {
          name_ = std::move(name);
        };

        inline std::shared_ptr<option::BaseOption> GetOption(size_t index) {
          return options_[index];
        };

        inline size_t GetOptionCount() {
          return options_.size();
        };

        [[nodiscard]] inline size_t GetSelectedOption() const {
          return selected_option_;
        };

        inline void SetSelectedOption(std::int64_t index) {
          if (index >= options_.size())
            index = 0;
          if (index < 0)
            index = static_cast<std::int64_t>(options_.size()) - 1;
          selected_option_ = index;
        };

        template<typename T>
        inline std::shared_ptr<T> AddOption(T option) {
          auto tmp = std::make_shared<T>(std::forward<T>(option));
          options_.push_back(tmp);

          return tmp;
        }

        inline void HandleKey(KeyInput key) {
          if (key == KeyInput::kUp) {
            if (selected_option_ > 0) {
              selected_option_--;
            } else {
              selected_option_ = options_.size() - 1;
            }
          } else if (key == KeyInput::kDown) {
            if (selected_option_ < options_.size() - 1) {
              selected_option_++;
            } else {
              selected_option_ = 0;
            }
          } else {
            GetOption(GetSelectedOption())->HandleKey(key);
          }
        };

      private:
        std::string name_{};
        std::vector<std::shared_ptr<option::BaseOption>> options_;
        std::int64_t selected_option_ = 0;
      };
    }
  }
}
#endif //GTA_BASE_SUBMENU_HPP