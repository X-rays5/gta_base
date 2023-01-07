//
// Created by X-ray on 12/16/22.
//

#pragma once
#ifndef GTA_BASE_DISABLED_OPTION_HPP
#define GTA_BASE_DISABLED_OPTION_HPP
#include "base_option.hpp"

namespace gta_base::ui::option {
class DisabledOption : public BaseOption {
 public:
  explicit DisabledOption(const std::string &label_key) {
    name_key_ = label_key;
  }

  void HandleKey(KeyInput key) final {

  }

  bool HasFlag(OptionFlag flag) final {
    if (flag == OptionFlag::kDisabled) {
      return true;
    }

    return false;
  }
};
}
#endif //GTA_BASE_DISABLED_OPTION_HPP
