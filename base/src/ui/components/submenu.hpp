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

        Submenu(std::string name_key, Submenus submenu_id, constructor_cb cb) : name_key_(std::move(name_key)), create_options_(std::move(cb)) {}

        [[nodiscard]] inline std::string GetName() const {
          return std::string((*kTRANSLATION_MANAGER)[name_key_]);
        };

        inline void SetNameKey(std::string name) {
          name_key_ = std::move(name);
        };

        inline std::shared_ptr<option::BaseOption> GetOption(size_t index) {
          return options_[index];
        };

        inline size_t GetOptionCount() {
          return options_.size();
        };

        [[nodiscard]] inline size_t GetSelectedOption() {
          if (selected_option_ >= options_.size())
            selected_option_ = 0;

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
          if (options_.empty())
            return;

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

        void Clear() {
          options_.clear();
        }

        void CreateOptions() {
          std::invoke(create_options_, this);
        }

      private:
        std::string name_key_{};
        constructor_cb create_options_;
        std::vector<std::shared_ptr<option::BaseOption>> options_;
        std::int64_t selected_option_ = 0;
      };
    }
  }
}
#endif //GTA_BASE_SUBMENU_HPP