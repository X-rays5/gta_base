//
// Created by X-ray on 3/12/2022.
//

#pragma once

#ifndef GTA_BASE_SUBMENUOPTION_HPP
#define GTA_BASE_SUBMENUOPTION_HPP
#include "baseoption.hpp"
#include "../../manager.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      namespace option {
        class SubmenuOption : public BaseOption {
        public:
          using action_t = void(*)();

          SubmenuOption(const std::string& name_key, const std::string& description_key, Submenus sub_id, action_t action = nullptr) {
            name_key_ = name_key;
            description_key_ = description_key;
            right_text_key_ = ">";
            sub_id_ = sub_id;
            action_ = action;
          }

          void HandleKey(KeyInput key) final {
            if (key == KeyInput::kReturn) {
              kMANAGER->SetActiveSubmenu(sub_id_);
              if (action_)
                action_();
            }

            if (key == KeyInput::kHotkey && action_)
              action_();
          }

          bool HasFlag(OptionFlag flag) final {
            return flag == OptionFlag::kSubmenuLink || flag == OptionFlag::kRightText || flag == OptionFlag::kHotkeyable;
          }

        private:
          Submenus sub_id_;
          action_t action_;
        };
      }
    }
  }
}
#endif //GTA_BASE_SUBMENUOPTION_HPP