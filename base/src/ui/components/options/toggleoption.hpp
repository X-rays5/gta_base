//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLEOPTION_HPP
#define GTA_BASE_TOGGLEOPTION_HPP
#include "baseoption.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      namespace option {
        class ToggleOption : public BaseOption {
        public:
          explicit ToggleOption(const std::string& name, const std::string& description, bool* toggle) {
            name_ = name;
            description_ = description;
            toggle_ = toggle;
          }

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
              return true;
            } else if (flag == OptionFlag::kToggle) {
              return true;
            } else if (flag == OptionFlag::kToggled) {
              return *toggle_;
            }

            return false;
          }
        private:
          bool* toggle_;
        };
      }
    }
  }
}
#endif //GTA_BASE_TOGGLEOPTION_HPP