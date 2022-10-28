//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_RENDER_SCRIPT_HPP
#define GTABASE_RENDER_SCRIPT_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base::scripts {
    class UiDraw : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kRenderer;
      }

      void Init() final;
      void RunTick() final;

    private:
      std::size_t last_idx_{};
    };
  }
#endif //GTABASE_RENDER_SCRIPT_HPP
