//
// Created by X-ray on 08/02/22.
//

#pragma once
#ifndef GTA_BASE_UITICK_HPP
#define GTA_BASE_UITICK_HPP
#include <string>
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class UiTick : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kScripting;
      }

      void Init() final;
      void RunTick() final;

    private:
      float test_value_f = 1;
      bool test_value_b = false;
      std::vector<std::string> test_value_v = {"test", "test2"};
      std::size_t test_value_v_idx = 0;

      float run_speed{};
    };
  }
}
#endif //GTA_BASE_UITICK_HPP
