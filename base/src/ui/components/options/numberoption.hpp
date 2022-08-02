//
// Created by X-ray on 07/29/22.
//

#pragma once
#ifndef GTA_BASE_NUMBEROPTION_HPP
#define GTA_BASE_NUMBEROPTION_HPP
#include "baseoption.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      namespace option {
        class NumberOption : public BaseOption {
        public:
          explicit NumberOption(const std::string& name, const std::string& description, float* value, float step, float min, float max) {
            name_ = name;
            description_ = description;
            value_ = value;
            step_ = step;
            min_ = min;
            max_ = max;
            UpdateRightText();
          }

          void HandleKey(KeyInput key) final {
            if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
              SendEvent(Event::kSelect);
            } else if (key == KeyInput::kLeft) {
              if (*value_ == min_)
                return;

              *value_ -= step_;
              if (*value_ < min_)
                *value_ = min_;
              UpdateRightText();
              SendEvent(Event::kChange);
            } else if (key == KeyInput::kRight) {
              if (*value_ == max_)
                return;

              *value_ += step_;
              if (*value_ > max_)
                *value_ = max_;
              UpdateRightText();
              SendEvent(Event::kChange);
            }
          }

          bool HasFlag(OptionFlag flag) final {
            if (flag == OptionFlag::kRightText) {
              return right_text_.empty();
            } else if (flag == OptionFlag::kRightIcon) {
              return icon_path_.string().empty();
            } else if (flag == OptionFlag::kHotkeyable) {
              return true;
            } else {
              return false;
            }
          }
        private:
          float* value_;
          float step_;
          float min_;
          float max_;

        private:
          void UpdateRightText() {
            auto tmp_right_text = fmt::format("[{:f}", *value_);
            while (!tmp_right_text.empty() && tmp_right_text.back() == '0')
              tmp_right_text.pop_back();
            if (!tmp_right_text.empty() && tmp_right_text.back() == '.')
              tmp_right_text += "0";
            tmp_right_text += "]";

            SetRightText(tmp_right_text);
          }
        };
      }
    }
  }
}
#endif //GTA_BASE_NUMBEROPTION_HPP