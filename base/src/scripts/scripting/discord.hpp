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
        return scriptmanager::ScriptType::kScripting;
      }

      void Init() final;
      void RunTick() final;

    private:
      std::uint64_t last_update_ = INT64_MAX; // so it refreshes the first time
    };
  }
}
#endif //GTA_BASE_DISCORD_HPP
