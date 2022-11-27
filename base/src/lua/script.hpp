//
// Created by X-ray on 11/27/22.
//

#pragma once
#ifndef GTA_BASE_LUA_SCRIPT_HPP
#define GTA_BASE_LUA_SCRIPT_HPP
#include <sol/sol.hpp>

namespace gta_base::lua {
  class Script {
  public:
    explicit Script(const std::filesystem::path& script_path);
    ~Script();

    void Tick();

    sol::state* GetState() {
      return &lua_state_;
    }

  private:
    bool has_tick_func_ = true;
    sol::state lua_state_;

  private:
    void LoadLibraries();
    void SetExceptionHandler();
    void AddFunctions();
    sol::table CreateReadOnlyTable(const std::string& name, sol::table& meta_table);

    static int Panic(lua_State* L);
    static void SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val);
    static std::string GetInternalLuaVar(lua_State* L, const std::string& name);
    static std::string GetCurrentFile(lua_State* L);

    static int GetCurrentLine(lua_State* L);
    static int ReadOnly(lua_State* L);
  };
}
#endif //GTA_BASE_LUA_SCRIPT_HPP
