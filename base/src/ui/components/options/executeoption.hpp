//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_EXECUTEOPTION_HPP
#define GTA_BASE_EXECUTEOPTION_HPP
#include <utility>

#include "baseoption.hpp"

#define UI_EXECUTE_OPTION_ACTION_AND_ID(action) action, #action

namespace gta_base {
  namespace ui {
    namespace option {
      class ExecuteOption : public BaseOption {
      public:
        using action_t = std::function<void()>;

        explicit ExecuteOption(const std::string& name_key, const std::string& description_key = "", action_t action = []{}, bool hotkeyable = true) :
          BaseOption(name_key, description_key, "", "", "", hotkeyable), action_(std::move(action))
        {}

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
            return right_text_key_.empty();
          } else if (flag == OptionFlag::kRightIcon) {
            return icon_path_.string().empty();
          } else if (flag == OptionFlag::kHotkeyable) {
            return hotkeyable_;
          } else {
            return false;
          }
        }
      private:
        action_t action_;
      };
    }
  }
}
#endif //GTA_BASE_EXECUTEOPTION_HPP