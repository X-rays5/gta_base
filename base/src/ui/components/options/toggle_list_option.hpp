//
// Created by X-ray on 08/26/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLE_LIST_OPTION_HPP
#define GTA_BASE_TOGGLE_LIST_OPTION_HPP
#include "base_option.hpp"

namespace gta_base::ui::option {
template<typename T>
class ToggleListOption : public BaseOption {
 public:
  explicit ToggleListOption(const std::string &name_key, const std::string &description_key, bool &toggle, std::size_t &idx, std::vector<T> &items, bool save_able = true, bool hotkey_able = true) :
      BaseOption(name_key, description_key, "", "", save_able, hotkey_able), toggle_(&toggle), idx_(&idx), items_(&items) {
    UpdateRightText();
  }

  void HandleKey(KeyInput key) final {
    if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
      *toggle_ = !*toggle_;
      SendEvent(Event::kSelect);
    } else if (key == KeyInput::kLeft) {
      if (*idx_ > 0) {
        *idx_ -= 1;
        UpdateRightText();
        SendEvent(Event::kChange);
      } else {
        *idx_ = items_->size() - 1;
        UpdateRightText();
        SendEvent(Event::kChange);
      }
    } else if (key == KeyInput::kRight) {
      if (*idx_ < items_->size() - 1) {
        *idx_ += 1;
        UpdateRightText();
        SendEvent(Event::kChange);
      } else {
        *idx_ = 0;
        UpdateRightText();
        SendEvent(Event::kChange);
      }
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
    } else if (flag == OptionFlag::kSaveable) {
      return save_able_;
    }

    return false;
  }

  std::string GetSaveVal() final {
    return fmt::format("{}##{}", *idx_, static_cast<int>(*toggle_));
  }

  void SetSavedVal(const std::string &val) final {
    if (auto pos = val.find("##"); pos != std::string::npos) {
      *toggle_ = std::stoi(val.substr(pos + 2));
      std::size_t tmp = std::stoull(val.substr(0, pos));
      if (tmp < items_->size())
        *idx_ = tmp;
      else
        LOG_WARN("{}: invalid idx saved", GetName());
    }
  }

 private:
  bool *toggle_;
  std::size_t *idx_;
  std::vector<T> *items_;

 private:
  void UpdateRightText() {
    SetRightTextKey(fmt::format("{} [{}/{}]", items_->at(*idx_), *idx_ + 1, items_->size()));
  }
};
}
#endif //GTA_BASE_TOGGLE_LIST_OPTION_HPP
