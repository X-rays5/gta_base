//
// Created by X-ray on 07/29/22.
//

#pragma once
#ifndef GTA_BASE_NUMBEROPTION_HPP
#define GTA_BASE_NUMBEROPTION_HPP
#include "baseoption.hpp"
#include "../../../misc/common.hpp"
#include "../keyboard.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      namespace option {
        template<typename T>
        requires std::integral<T> or std::floating_point<T>
        class NumberOption : public BaseOption {
        public:
          explicit NumberOption(const std::string& name_key, const std::string& description_key, T& value, T step, T min, T max) {
            name_key_ = name_key;
            description_key_ = description_key;
            value_ = &value;
            step_ = step;
            min_ = min;
            max_ = max;
            UpdateRightText();
          }

          void HandleKey(KeyInput key) final {
            if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
              keyboard::kMANAGER->ShowKeyboard(std::to_string(common::GetEpoch()), [this](const std::string& text, keyboard::Result res){
                if (text.empty())
                  return;

                if (res == keyboard::Result::kDone) {
                  try {
                    constexpr static const bool is_float = std::is_floating_point_v<T>;
                    if constexpr (is_float) {
                      *value_ = static_cast<T>(std::stod(text));
                    } else {
                      *value_ = static_cast<T>(std::stoll(text));
                    }
                    /*CheckValueBounds();
                    UpdateRightText();
                    SendEvent(Event::kChange);*/
                  } catch (std::exception& e) {
                    LOG_WARN("{}", e.what());
                  }
                }
              });
            } else if (key == KeyInput::kLeft) {
              if (*value_ == min_) {
                *value_ = max_;
                UpdateRightText();
                SendEvent(Event::kChange);
                return;
              }

              *value_ -= step_;
              CheckValueBounds();
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
              CheckValueBounds();
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
              return true;
            } else {
              return false;
            }
          }
        private:
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

          inline void CheckValueBounds() {
            if (*value_ < min_)
              *value_ = min_;
            else if (*value_ > max_)
              *value_ = max_;
          }
        };
      }
    }
  }
}
#endif //GTA_BASE_NUMBEROPTION_HPP