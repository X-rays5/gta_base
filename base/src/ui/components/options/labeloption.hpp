//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_LABELOPTION_HPP
#define GTA_BASE_LABELOPTION_HPP
#include <format>
#include "baseoption.hpp"

namespace gta_base {
  namespace ui {
    namespace option {
      class LabelOption : public BaseOption {
      public:
        explicit LabelOption(const std::string& label_key) {
          center_text_key_ = label_key;
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