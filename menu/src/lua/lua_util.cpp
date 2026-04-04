//
// Created by X-ray on 04/04/2026.
//

#include "lua_util.hpp"
#include <base-common/fs/vfs.hpp>

namespace base::menu::lua {
  std::string StackValueToString(lua_State* L, const std::int32_t index) {
    return luaL_tolstring(L, index, nullptr);
  }

  sol::table CreateReadOnlyTable(sol::state& L, const std::string& name, sol::table& meta_table) {
    auto table = L.create_named_table(name);
    meta_table[sol::meta_function::new_index] = ReadOnly;
    meta_table[sol::meta_function::index] = meta_table;

    table[sol::metatable_key] = meta_table;

    return table;
  }

  std::optional<sol::protected_function_result> CallLuaFunction(sol::state& L, const std::string& fn_name) {
    try {
      return sol::protected_function(L[fn_name])();
    } catch (sol::error& e) {
      LOG_ERROR("LUA: {}", e.what());
    } catch (std::exception& e) {
      LOG_ERROR(e.what());
    } catch (...) {
      LOG_ERROR("Unknown exception occurred.");
    }

    return std::nullopt;
  }

  int ReadOnly(lua_State* L) {
    return luaL_error(L, "This table is read only");
  }

  void SetInternalLuaVar(const sol::state& L, const std::string& name, const std::string& val) {
    lua_getglobal(L, "__SCRIPT_INFO__");
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      lua_newtable(L);
      lua_setglobal(L, "__SCRIPT_INFO__");
      lua_getglobal(L, "__SCRIPT_INFO__");
    }
    lua_pushstring(L, val.c_str());
    lua_setfield(L, -2, name.c_str());
    lua_pop(L, 1);
  }

  std::string GetInternalLuaVar(const sol::state& L, const std::string& name) {
    lua_getglobal(L, "__SCRIPT_INFO__");
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      return "";
    }
    lua_getfield(L, -1, name.c_str());
    std::string result = StackValueToString(L, -1);
    lua_pop(L, 2);

    return result;
  }
  
  std::string GetCurrentFile(const sol::state& L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "S", &ar);
    std::string file = std::filesystem::path(ar.source).filename().string();

    return file;
  }

  int GetCurrentLine(const sol::state& L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "l", &ar);

    return ar.currentline;
  }

  std::string GetScriptName(const sol::state& L) {
    return GetInternalLuaVar(L, "script_name");
  }

  void SetScriptName(const sol::state& L, const std::string& name) {
    SetInternalLuaVar(L, "script_name", name);
  }

  std::filesystem::path GetScriptPath(const sol::state& L) {
    return GetInternalLuaVar(L, "script_dir");
  }
  void SetScriptPath(const sol::state& L, const std::filesystem::path& path) {
    SetInternalLuaVar(L, "script_dir", path.string());
  }

  std::filesystem::path GetMainFile(const sol::state& L) {
    return GetInternalLuaVar(L, "script_main_file");
  }
  void SetMainFile(const sol::state& L, const std::filesystem::path& path) {
    SetInternalLuaVar(L, "script_main_file", path.string());
  }

  std::vector<std::filesystem::path> GetLuaScripts() {
    std::vector<std::filesystem::path> paths;
    const std::filesystem::path scripts_dir = common::fs::vfs::GetLuaScripts();

    std::filesystem::directory_iterator it(scripts_dir);
    const std::filesystem::directory_iterator end;
    for (; it != end; ++it) {
      std::filesystem::path path = it->path();
      paths.emplace_back(path);
    }

    return paths;
  }
}
