//
// Created by X-ray on 02/22/23.
//

#pragma once
#ifndef GTA_BASE_LIST_OPTION_8DB3A821FB1D46439314576F6BEA163D_HPP
#define GTA_BASE_LIST_OPTION_8DB3A821FB1D46439314576F6BEA163D_HPP
#include "base_option.hpp"

namespace gta_base::ui::draw::components {
  template<typename arr_t, typename access_t> requires common::has_array_operator<arr_t, access_t> and common::can_convert_to_string<access_t> and std::is_arithmetic_v<access_t> and common::has_size_method<arr_t>
  class ListOption : public BaseOption {
  public:
    ListOption(std::string name, std::string description, arr_t& array, access_t& arr_idx, flags::OptionFlags flags = flags::OptionFlags::kNone) :
    array_(std::move(array)), arr_size_(array_->size()), arr_idx_(arr_idx),
    BaseOption(name, description, nullptr, flags::OptionFlags::kSaveAble | flags)
    {
    }

    std::string Serialize() override {
      return common::ToString(*arr_idx_);
    }

    void Deserialize(const std::string& val) override {
      arr_idx_ = common::LexicalCast<access_t>(val);
      if (arr_idx_ < 0)
        arr_idx_ = 0;
      else if (arr_idx_ >= arr_size_)
        arr_idx_ = arr_size_ - 1;
    }

    void HandleKey(key_input::KeyBinds key_bind) override {
      switch (key_bind) {
        case key_input::KeyBinds::ui_select: {
          SendEvent(OptionEvent::kSelect);
          break;
        }
        case key_input::KeyBinds::ui_left: {
          ScrollLeft();
          break;
        }
        case key_input::KeyBinds::ui_right: {
          ScrollRight();
          break;
        }
        default:
      }
    }

  protected:
    arr_t& array_;
    access_t& arr_idx_{};
    std::size_t arr_size_{};

  protected:
    inline void ScrollLeft() {
      if (arr_idx_ == 0)
        arr_idx_ = arr_size_ - 1;
      else
        arr_idx_ -= 1;
    }

    inline void ScrollRight() {
      if (arr_idx_ == arr_size_ - 1)
        arr_idx_ = 0;
      else
        arr_idx_ += 1;
    }
  };
}
#endif //GTA_BASE_LIST_OPTION_8DB3A821FB1D46439314576F6BEA163D_HPP
