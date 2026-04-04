//
// Created by X-ray on 04/04/2026.
//

#pragma once

#pragma warning(push)
#pragma warning(disable : 5321)
#include <sol/sol.hpp>
#pragma warning(pop)

namespace base::menu::lua {
  std::string StackValueToString(lua_State* L, std::int32_t index);

  sol::table CreateReadOnlyTable(sol::state& L, const std::string& name, sol::table& meta_table);

  std::optional<sol::protected_function_result> CallLuaFunction(sol::state& L, const std::string& fn_name);

  int ReadOnly(lua_State* L);

  void SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val);

  std::string GetInternalLuaVar(lua_State* L, const std::string& name);

  std::string GetCurrentFile(lua_State* L);
  int GetCurrentLine(lua_State* L);

  std::string GetScriptName(lua_State* L);
  void SetScriptName(lua_State* L, const std::string& name);

  std::filesystem::path GetScriptPath(lua_State* L);
  void SetScriptPath(lua_State* L, const std::filesystem::path& path);

  std::filesystem::path GetMainFile(lua_State* L);
  void SetMainFile(lua_State* L, const std::filesystem::path& path);

  std::vector<std::filesystem::path> GetLuaScripts();

  [[nodiscard]] sol::protected_function_result RunScriptFileSafe(sol::state& sol, const std::filesystem::path& path);
}