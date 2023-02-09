//
// Created by X-ray on 02/07/23.
//

#include "lua.hpp"
#include "../../lua/manager.hpp"

namespace gta_base::scripts {

  void LuaScriptThread::Init() {

  }

  void LuaScriptThread::RunTick() {
    if (lua::kMANAGER)
      lua::kMANAGER->RunScriptTick();
  }
}