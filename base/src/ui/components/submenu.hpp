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
#include <mutex>
#include "options/baseoption.hpp"
#include "enums.hpp"

namespace gta_base::ui {
    class Submenu {
    public:
      using constructor_cb = std::function<void(Submenu*)>;

      Submenu(std::string name_key, constructor_cb cb) : name_key_(std::move(name_key)), create_options_(std::move(cb)) {}

      [[nodiscard]] inline std::string GetName() {
        std::unique_lock lock(mtx_);
        return std::string((*kTRANSLATION_MANAGER)[name_key_]);
      };

      inline void SetNameKey(std::string name) {
        std::unique_lock lock(mtx_);
        name_key_ = std::move(name);
      };

      inline std::shared_ptr<option::BaseOption> GetOption(size_t index) {
        std::unique_lock lock(mtx_);
        if (index < options_.size()) {
          return options_[index];
        }

        return nullptr;
      };

      inline size_t GetOptionCount() {
        std::unique_lock lock(mtx_);
        return options_.size();
      };

      [[nodiscard]] inline size_t GetSelectedOption() {
        std::unique_lock lock(mtx_);
        if (selected_option_ >= options_.size())
          selected_option_ = 0;

        return selected_option_;
      };

      inline void SetSelectedOption(std::int64_t index) {
        std::unique_lock lock(mtx_);
        if (index >= options_.size())
          index = 0;
        if (index < 0)
          index = static_cast<std::int64_t>(options_.size()) - 1;
        selected_option_ = index;
      };

      template<typename T>
      inline std::shared_ptr<T> AddOption(T option) {
        try {
          std::unique_lock lock(mtx_);
          auto tmp = std::make_shared<T>(std::forward<T>(option));
          options_.push_back(tmp);

          return tmp;
        } catch (std::exception& e) {
          LOG_ERROR("{}: {}", typeid(std::current_exception()).name(), e.what());
        }

        return nullptr;
      }

      inline void HandleKey(KeyInput key) {
        std::unique_lock lock(mtx_);
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
        std::unique_lock lock(mtx_);
        options_.clear();
      }

      void CreateOptions() {
        std::unique_lock lock(mtx_);
        std::invoke(create_options_, this);
      }

      bool HotkeyPressed(const std::string& key) {
        std::unique_lock lock(mtx_);
        if (options_.empty()) {
          CreateOptions();
          if (options_.empty())
            return false;
        }

        for (auto&& opt : options_) {
          if (opt->GetNameKey() == key) {
            if (opt->HasFlag(OptionFlag::kHotkeyable)) {
              opt->HandleKey(KeyInput::kHotkey);

              options_.clear();
              return true;
            }
          }
        }

        options_.clear();
        return false;
      }

    private:
      std::recursive_mutex mtx_;
      std::string name_key_{};
      constructor_cb create_options_;
      std::vector<std::shared_ptr<option::BaseOption>> options_;
      std::int64_t selected_option_ = 0;
    };
  }
#endif //GTA_BASE_SUBMENU_HPP