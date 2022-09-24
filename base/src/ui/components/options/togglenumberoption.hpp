//
// Created by X-ray on 08/22/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLENUMBEROPTION_HPP
#define GTA_BASE_TOGGLENUMBEROPTION_HPP
#include "baseoption.hpp"

namespace gta_base {
  namespace ui {
    namespace option {
      template<typename T>
      requires std::integral<T> or std::floating_point<T>
      class ToggleNumberOption : public BaseOption {
      public:
        explicit ToggleNumberOption(const std::string& name_key, const std::string& description_key, bool& toggle, T& value, T step, T min, T max, bool hotkeyable = true) :
          BaseOption(name_key, description_key, "", "", "", hotkeyable), toggle_(&toggle), value_(&value), step_(step), min_(min), max_(max)
        {
          UpdateRightText();
        }

        void HandleKey(KeyInput key) final {
          if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
            *toggle_ = !*toggle_;
            UpdateRightText();
            SendEvent(Event::kSelect);
          } else if (key == KeyInput::kLeft) {
            if (*value_ == min_) {
              *value_ = max_;
              UpdateRightText();
              SendEvent(Event::kChange);
              return;
            }

            *value_ -= step_;
            if (*value_ < min_)
              *value_ = min_;
            UpdateRightText();
            SendEvent(Event::kChange);
          } else if (key == KeyInput::kRight) {
            if (*value_ == max_) {
              *value_ = min_;
              UpdateRightText();
              SendEvent(Event::kChange);
              return;
            }

            *value_ += step_;
            if (*value_ > max_)
              *value_ = max_;
            UpdateRightText();
            SendEvent(Event::kChange);
          }
        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kRightText) {
            return right_text_key_.empty();
          } else if (flag == OptionFlag::kRightIcon) {
            return icon_path_.string().empty();
          } else if (flag == OptionFlag::kHotkeyable) {
            return hotkeyable_;
          } else if (flag == OptionFlag::kToggle) {
            return true;
          } else if (flag == OptionFlag::kToggled) {
            return *toggle_;
          } else {
            return false;
          }
        }
      private:
        bool* toggle_;
        T* value_;
        T step_;
        T min_;
        T max_;

      private:
        void UpdateRightText() {
          std::string tmp_right_text;

          constexpr static const bool is_float = std::is_floating_point_v<T>;
          if constexpr (is_float) {
            tmp_right_text = fmt::format("[{:.3f}]", *value_);
          } else {
            tmp_right_text = fmt::format("[{}]", *value_);
          }

          SetRightTextKey(tmp_right_text);
        }
      };
    }
  }
}
#endif //GTA_BASE_TOGGLENUMBEROPTION_HPP
