//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_RENDER_SCRIPT_HPP
#define GTABASE_RENDER_SCRIPT_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class UiDraw : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kRenderer;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTABASE_RENDER_SCRIPT_HPP
