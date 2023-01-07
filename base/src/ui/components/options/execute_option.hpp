//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_EXECUTE_OPTION_HPP
#define GTA_BASE_EXECUTE_OPTION_HPP
#include <utility>

#include "base_option.hpp"

namespace gta_base::ui::option {
class ExecuteOption : public BaseOption {
 public:
  using action_t = std::function<void()>;

  explicit ExecuteOption(const std::string &name_key, const std::string &description_key = "", action_t action = [] {}, bool hotkey_able = true) :
      BaseOption(name_key, description_key, "", "", false, hotkey_able), action_(std::move(action)) {
    if (!action_) {
      hotkey_able_ = false;
    }
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
      return right_text_key_.empty();
    } else if (flag == OptionFlag::kHotkeyable) {
      return hotkey_able_;
    }

    return false;
  }
 private:
  action_t action_;
};
}
#endif //GTA_BASE_EXECUTE_OPTION_HPP