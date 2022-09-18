//
// Created by X-ray on 08/26/22.
//

#pragma once
#ifndef GTA_BASE_TOGGLELISTOPTION_HPP
#define GTA_BASE_TOGGLELISTOPTION_HPP
#include "baseoption.hpp"

namespace gta_base {
  namespace ui {
    namespace option {
      template<typename T>
      class ToggleListOption : public BaseOption {
      public:
        explicit ToggleListOption(const std::string& name_key, const std::string& description_key, bool& toggle, std::size_t& idx, std::vector<T>& items) :
          BaseOption(name_key, description_key), toggle_(&toggle), idx_(&idx), items_(&items)
        {
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
          } else if (flag == OptionFlag::kRightIcon) {
            return icon_path_.string().empty();
          } else if (flag == OptionFlag::kHotkeyable) {
            return true;
          } else if (flag == OptionFlag::kToggle) {
            return true;
          } else if (flag == OptionFlag::kToggled) {
            return *toggle_;
          } else {
            return false;
          }
        }
      private:
        bool* toggle_;
        std::size_t* idx_;
        std::vector<T>* items_;

      private:
        void UpdateRightText() {
          SetRightTextKey(fmt::format("{} [{}/{}]", items_->at(*idx_), *idx_ + 1, items_->size()));
        }
      };
    }
  }
}
#endif //GTA_BASE_TOGGLELISTOPTION_HPP
