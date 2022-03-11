//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_SUBMENU_HPP
#define GTA_BASE_SUBMENU_HPP
#include <vector>
#include "option/baseoption.hpp"
#include "enums.hpp"

namespace ui {
  namespace components {
    class Submenu {
    public:
      using constructor_cb = void(*)(Submenu *);

      explicit Submenu(const char* name, const char* description, Submenus submenu_id, constructor_cb) : name_((char*)name), description_((char*)description), submenu_id_(submenu_id) {
        constructor_cb(this);
      }

      [[nodiscard]] char* GetName() const {
        return name_;
      };

      void SetName(const char* name) {
        name_ = (char*)name;
      };

      [[nodiscard]] char* GetDescription() const {
        return description_;
      };

      void SetDescription(char* description) {
        description_ = description;
      };

      std::shared_ptr<option::BaseOption> GetOption(size_t index) {
        return options_[index - 1];
      };

      size_t GetOptionCount() {
        return options_.size();
      };

      [[nodiscard]] size_t GetSelectedOption() const {
        return selected_option_;
      };

      void SetSelectedOption(size_t index) {
        if (index > GetOptionCount())
          index = 1;
        selected_option_ = index;
      };

      void AddOption(const std::shared_ptr<option::BaseOption>& option) {
        options_.emplace_back(option);
      };

      template<typename T, typename... Args>
      void AddOption(Args&&... args) {
        options_.push_back(std::make_shared<T>(std::forward<Args>(args)...));
      }

      void HandleKey(KeyInput key) {
        if (key == KeyInput::kUp) {
          SetSelectedOption(selected_option_ + 1);
        } else if (key == KeyInput::kDown) {
          SetSelectedOption(selected_option_ - 1);
        } else {
          GetOption(GetSelectedOption())->HandleKey(key);
        }
      };

    private:
      char* name_{};
      char* description_{};
      std::vector<std::shared_ptr<option::BaseOption>> options_;
      size_t selected_option_{};
      Submenus submenu_id_;
    };
  }
}
#endif //GTA_BASE_SUBMENU_HPP
