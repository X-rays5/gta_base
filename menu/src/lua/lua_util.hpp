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

  void SetInternalLuaVar(const sol::state& L, const std::string& name, const std::string& val);

  std::string GetInternalLuaVar(const sol::state& L, const std::string& name);

  std::string GetCurrentFile(const sol::state& L);
  int GetCurrentLine(const sol::state& L);

  std::string GetScriptName(const sol::state& L);
  void SetScriptName(const sol::state& L, const std::string& name);

  std::filesystem::path GetScriptPath(const sol::state& L);
  void SetScriptPath(const sol::state& L, const std::filesystem::path& path);

  std::filesystem::path GetMainFile(const sol::state& L);
  void SetMainFile(const sol::state& L, const std::filesystem::path& path);

  std::vector<std::filesystem::path> GetLuaScripts();
}