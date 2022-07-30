//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_LABELOPTION_HPP
#define GTA_BASE_LABELOPTION_HPP
#include <format>
#include "baseoption.hpp"

namespace ui {
  namespace components {
    namespace option {
      class LabelOption : public BaseOption {
      public:
        explicit LabelOption(const std::string& label) {
          center_text_ = std::format("[{}]", label);
        }

        void HandleKey(KeyInput key) final {

        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kLabel) {
            return true;
          } else {
            return false;
          }
        }
      };
    }
  }
}
#endif //GTA_BASE_LABELOPTION_HPP
