//
// Created by X-ray on 08/22/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLE_NUMBER_OPTION_HPP
#define GTA_BASE_TOGGLE_NUMBER_OPTION_HPP
#include "base_option.hpp"

namespace gta_base::ui::option {
      template<typename T>
      requires std::integral<T> or std::floating_point<T>
      class ToggleNumberOption : public BaseOption {
      public:
        explicit ToggleNumberOption(const std::string& name_key, const std::string& description_key, bool& toggle, T& value, T step, T min, T max, bool save_able = true, bool hotkey_able = true) :
          BaseOption(name_key, description_key, "", "", save_able, hotkey_able), toggle_(&toggle), value_(&value), step_(step), min_(min), max_(max)
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
          } else if (key == KeyInput::kChangeValue) {
            T* value = value_;
            T min = min_;
            T max = max_;
            keyboard::kMANAGER->ShowKeyboard(std::to_string(common::GetEpoch()), [value, min, max](std::string text, keyboard::Result res) {
              if (res != keyboard::Result::kDone || text.empty())
                return;

              if (value) {
                FromString(value, text, min, max);
              }
            });
          }
        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kRightText) {
            return right_text_key_.empty();
          } else if (flag == OptionFlag::kHotkeyable) {
            return hotkey_able_;
          } else if (flag == OptionFlag::kToggle) {
            return true;
          } else if (flag == OptionFlag::kToggled) {
            return *toggle_;
          } else if (flag == OptionFlag::kSavable) {
            return save_able_;
          }

          return false;
        }

        std::string GetSaveVal() final {
          return fmt::format("{}##{}", *value_, static_cast<int>(*toggle_));
        }

        void SetSavedVal(const std::string& val) final {
          if (auto pos = val.find("##"); pos != std::string::npos) {
            *value_ = std::stod(val.substr(0, pos));
            *toggle_ = std::stoi(val.substr(pos + 2));
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

        inline static void FromString(T* out_val, const std::string& val, T min, T max) {
          auto tmp = common::RemoveNonNumerical(val);
          if (tmp.empty())
            return;

          *out_val = std::clamp(common::LexicalCast<T>(val), min, max);
        }
      };
    }
#endif //GTA_BASE_TOGGLE_NUMBER_OPTION_HPP
