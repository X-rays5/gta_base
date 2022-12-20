//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLE_OPTION_HPP
#define GTA_BASE_TOGGLE_OPTION_HPP
#include "base_option.hpp"

namespace gta_base::ui::option {
      class ToggleOption : public BaseOption {
      public:
        explicit ToggleOption(const std::string& name_key, const std::string& description_key, bool* toggle, bool save_able = true, bool hotkey_able = true) :
          BaseOption(name_key, description_key, "", "", save_able, hotkey_able), toggle_(toggle)
        {}

        void HandleKey(KeyInput key) final {
          if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
            *toggle_ = !*toggle_;

            SendEvent(Event::kSelect);
            SendEvent(Event::kChange);
          }
        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kHotkeyable) {
            return hotkey_able_;
          } else if (flag == OptionFlag::kToggle) {
            return true;
          } else if (flag == OptionFlag::kToggled) {
            return *toggle_;
          } else if (flag == OptionFlag::kSaveable) {
            return save_able_;
          }

          return false;
        }

        std::string GetSaveVal() final {
          return std::to_string(*toggle_);
        }

        void SetSavedVal(const std::string& val) final {
          *toggle_ = std::stoi(val);
        }
      private:
        bool* toggle_;
      };
    }
#endif //GTA_BASE_TOGGLE_OPTION_HPP