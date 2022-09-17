//
// Created by X-ray on 09/17/22.
//

#pragma once
#ifndef GTA_BASE_LOOPS_HPP
#define GTA_BASE_LOOPS_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class Loops : public scriptmanager::BaseScript {
    public:
      Loops() {
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
#endif //GTA_BASE_LOOPS_HPP
