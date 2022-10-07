//
// Created by X-ray on 3/12/2022.
//

#pragma once

#ifndef GTA_BASE_SUBMENUOPTION_HPP
#define GTA_BASE_SUBMENUOPTION_HPP
#include <utility>

#include "baseoption.hpp"
#include "../../manager.hpp"

namespace gta_base::ui::option {
      class SubmenuOption : public BaseOption {
      public:
        using action_t = std::function<void()>;

        SubmenuOption(const std::string& name_key, const std::string& description_key, Submenus sub_id, action_t action = nullptr, bool hotkeyable = true) :
          BaseOption(name_key, description_key, "", ">", "", false, hotkeyable), sub_id_(sub_id), action_(std::move(action))
        {}

        void HandleKey(KeyInput key) final {
          if (key == KeyInput::kReturn || key == KeyInput::kHotkey) {
            kMANAGER->SetActiveSubmenu(sub_id_);
            if (action_) {
              std::invoke(action_);
            }
          }
        }

        bool HasFlag(OptionFlag flag) final {
          if (flag == OptionFlag::kSubmenuLink)
            return true;
          else if (flag == OptionFlag::kRightText)
            return true;
          else if (flag == OptionFlag::kHotkeyable)
            return hotkeyable_;

          return false;
        }

      private:
        Submenus sub_id_;
        action_t action_;
      };
    }
#endif //GTA_BASE_SUBMENUOPTION_HPP