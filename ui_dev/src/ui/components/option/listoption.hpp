//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_LISTOPTION_HPP
#define GTA_BASE_LISTOPTION_HPP
#include "baseoption.hpp"

namespace ui {
  namespace components {
    namespace option {
      template<typename T>
      class ListOption : public BaseOption {
      public:
        using action_t = void(*)();

        explicit ListOption(const std::string& name, const std::string& description, std::size_t& idx, std::vector<T>& items) {
          name_ = name;
          description_ = description;
          idx_ = &idx;
          items_ = &items;
          UpdateRightText();
        }

        void HandleKey(KeyInput key) final {
          if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
            SendEvent(Event::kSelect);
          } else if (key == KeyInput::kLeft) {
            if (*idx_ > 0) {
              *idx_ -= 1;
              UpdateRightText();
              SendEvent(Event::kChange);
            }
          } else if (key == KeyInput::kRight) {
            if (*idx_ < items_->size() - 1) {
              *idx_ += 1;
              UpdateRightText();
              SendEvent(Event::kChange);
            }
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
        std::size_t* idx_;
        std::vector<T>* items_;

      private:
        void UpdateRightText() {
          SetRightText(std::format("{} [{}/{}]", items_->at(*idx_), *idx_ + 1, items_->size()));
        }
      };
    }
  }
}
#endif //GTA_BASE_LISTOPTION_HPP
