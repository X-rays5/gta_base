//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_GAME_SCRIPT_HPP
#define GTABASE_GAME_SCRIPT_HPP
#include "../../threads/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class Game : public threads::BaseScript {
    public:
      threads::ThreadType GetType() final {
        return threads::ThreadType::kGame;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTABASE_GAME_SCRIPT_HPP
