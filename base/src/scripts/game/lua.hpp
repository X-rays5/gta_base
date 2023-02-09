//
// Created by X-ray on 02/07/23.
//

#pragma once
#ifndef GTA_BASE_LUA_589851FE9FB2465D81E3542CF416937E_HPP
#define GTA_BASE_LUA_589851FE9FB2465D81E3542CF416937E_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base::scripts {
  class LuaScriptThread : public scriptmanager::BaseScript {
  public:
    LuaScriptThread() {
      initialized_ = true;
    }

    scriptmanager::ScriptType GetType() final {
      return scriptmanager::ScriptType::kGame;
    }

    void Init() final;
    void RunTick() final;
  };
}
#endif //GTA_BASE_LUA_589851FE9FB2465D81E3542CF416937E_HPP
