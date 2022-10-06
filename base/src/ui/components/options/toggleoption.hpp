//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLEOPTION_HPP
#define GTA_BASE_TOGGLEOPTION_HPP
#include "baseoption.hpp"

namespace gta_base::ui::option {
      class ToggleOption : public BaseOption {
      public:
        explicit ToggleOption(const std::string& name_key, const std::string& description_key, bool* toggle, bool saveable = true, bool hotkeyable = true) :
          BaseOption(name_key, description_key, "", "", "", saveable, hotkeyable), toggle_(toggle)
        {}

        void HandleKey(KeyInput key) final {
          if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
            *toggle_ = !*toggle_;

            SendEvent(Event::kSelect);
            SendEvent(Event::kChange);
          }
        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kRightIcon) {
            return true;
          } else if (flag == OptionFlag::kHotkeyable) {
            return hotkeyable_;
          } else if (flag == OptionFlag::kToggle) {
            return true;
          } else if (flag == OptionFlag::kToggled) {
            return *toggle_;
          } else if (flag == OptionFlag::kSavable) {
            return saveable_;
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
#endif //GTA_BASE_TOGGLEOPTION_HPP