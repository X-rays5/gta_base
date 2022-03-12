//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_EXECUTEOPTION_HPP
#define GTA_BASE_EXECUTEOPTION_HPP
#include "baseoption.hpp"

namespace ui {
  namespace components {
    namespace option {
      class ExecuteOption : public BaseOption {
      public:
        using action_t = void(*)();

        explicit ExecuteOption(const std::string& name, const std::string& description = "", action_t action = []{}) {
          name_ = name;
          description_ = description;
          action_ = action;
        }

        void HandleKey(KeyInput key) final {
          if (!action_)
            return;

          if (key == KeyInput::kReturn)
            action_();
        }
      private:
        action_t action_;
      };
    }
  }
}
#endif //GTA_BASE_EXECUTEOPTION_HPP
