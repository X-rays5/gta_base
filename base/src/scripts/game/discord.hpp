//
// Created by X-ray on 08/02/22.
//

#pragma once
#ifndef GTA_BASE_DISCORD_HPP
#define GTA_BASE_DISCORD_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace scripts {
    class Discord : public scriptmanager::BaseScript {
    public:
      scriptmanager::ScriptType GetType() final {
        return scriptmanager::ScriptType::kGame;
      }

      void Init() final;
      void RunTick() final;
    };
  }
}
#endif //GTA_BASE_DISCORD_HPP
