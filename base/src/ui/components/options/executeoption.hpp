//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_EXECUTEOPTION_HPP
#define GTA_BASE_EXECUTEOPTION_HPP
#include "baseoption.hpp"

#define UI_EXECUTE_OPTION_ACTION_AND_ID(action) action, #action

namespace gta_base {
  namespace ui {
    namespace components {
      namespace option {
        class ExecuteOption : public BaseOption {
        public:
          using action_t = void(*)();

          explicit ExecuteOption(const std::string& name, const std::string& description = "", action_t action = []{}, const std::string& action_id = "") {
            name_ = name;
            description_ = description;
            action_ = action;
            action_id_ = action_id;
          }

          void HandleKey(KeyInput key) final {
            if (!action_)
              return;

            if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
              action_();
              SendEvent(Event::kSelect);
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
          action_t action_;
          std::string action_id_;
        };
      }
    }
  }
}
#endif //GTA_BASE_EXECUTEOPTION_HPP