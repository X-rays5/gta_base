//
// Created by X-ray on 08/26/22.
//

#pragma once
#ifndef GTA_BASE_UI_DISABLE_CONTROL_HPP
#define GTA_BASE_UI_DISABLE_CONTROL_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class UIDisablePhone : public scriptmanager::BaseScript {
    public:
      UIDisablePhone() {
        initialized_ = true;
      }

      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kGame;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTA_BASE_UI_DISABLE_CONTROL_HPP
