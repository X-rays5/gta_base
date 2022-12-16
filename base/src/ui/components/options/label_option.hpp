//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_LABEL_OPTION_HPP
#define GTA_BASE_LABEL_OPTION_HPP
#include <format>
#include "base_option.hpp"

namespace gta_base::ui::option {
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
          }

          return false;
        }
      };
    }
#endif //GTA_BASE_LABEL_OPTION_HPP