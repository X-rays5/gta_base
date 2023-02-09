//
// Created by X-ray on 11/27/22.
//

#pragma once
#ifndef GTA_BASE_LUA_SCRIPT_HPP
#define GTA_BASE_LUA_SCRIPT_HPP
#include <sol/sol.hpp>
#include "logger.hpp"

namespace gta_base::lua {
  class Script {
  public:
    explicit Script(const std::string& name, const std::filesystem::path& script_dir, const std::filesystem::path& main_file);
    ~Script();

    void Init();
    void Tick();

    sol::state* GetState() {
      return &lua_state_;
    }

  private:
    bool first_tick_ = true;
    bool has_tick_func_ = true;
    Logger logger_;
    sol::state lua_state_;

  private:
    void LoadLibraries();
    void SetExceptionHandler();
    void AddFunctions();
    sol::table CreateReadOnlyTable(const std::string& name, sol::table& meta_table);

    static void SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val);
    static std::string GetInternalLuaVar(lua_State* L, const std::string& name);
    static std::string GetCurrentFile(lua_State* L);

    std::optional<sol::protected_function_result> CallLuaFunction(sol::state& L, const std::string& fn_name);

    std::string GetScriptName();

    void SetScriptName(const std::string& name);
    std::filesystem::path GetScriptPath();

    void SetScriptPath(const std::filesystem::path& path);
    std::filesystem::path GetMainFile();

    void SetMainFile(const std::filesystem::path& path);

    static int GetCurrentLine(lua_State* L);
    static int ReadOnly(lua_State* L);

    static inline std::string StackValueToString(lua_State* L, std::size_t index) {
      return luaL_tolstring(L, index, nullptr);
    }

    static std::string FormatLuaVariadicArgs(const std::string& format, sol::variadic_args va);
  };
}
#endif //GTA_BASE_LUA_SCRIPT_HPP
