//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_LIST_OPTION_HPP
#define GTA_BASE_LIST_OPTION_HPP
#include "base_option.hpp"

namespace gta_base::ui::option {
      template<typename T>
      class ListOption : public BaseOption {
      public:
        explicit ListOption(const std::string& name_key, const std::string& description_key, std::size_t& idx, std::vector<T>& items, bool save_able = true, bool hotkey_able = true) :
          BaseOption(name_key, description_key, "", "", save_able, hotkey_able), idx_(&idx), items_(&items)
        {
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
          } else if (flag == OptionFlag::kSavable) {
            return save_able_;
          }

          return false;
        }

        std::string GetSaveVal() final {
          return std::to_string(*idx_);
        }

        void SetSavedVal(const std::string& val) final {
          std::size_t tmp = std::stoull(val);
          if (tmp < items_->size())
            *idx_ = tmp;
          else
            LOG_WARN("{}: invalid idx saved", GetName());
        }

      private:
        std::size_t* idx_;
        std::vector<T>* items_;

      private:
        void UpdateRightText() {
          SetRightTextKey(fmt::format("{} [{}/{}]", items_->at(*idx_), *idx_ + 1, items_->size()));
        }
      };
    }
#endif //GTA_BASE_LIST_OPTION_HPP