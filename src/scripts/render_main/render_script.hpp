//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_RENDER_SCRIPT_HPP
#define GTABASE_RENDER_SCRIPT_HPP
#include "../../threads/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class Render : public threads::BaseScript {
    public:
      threads::ThreadType GetType() final {
        return threads::ThreadType::kRenderer;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTABASE_RENDER_SCRIPT_HPP
