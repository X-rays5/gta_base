//
// Created by X-ray on 04/04/2026.
//

#include "lua_util.hpp"
#include <base-common/fs/vfs.hpp>

namespace base::menu::lua {
  namespace {
    sol::protected_function_result SafeScriptErrorHandler(lua_State* L, sol::protected_function_result result) {
      const std::string script_name = GetScriptName(L);

      const sol::type t = sol::type_of(L, result.stack_index());
      auto err = fmt::format("A '{}' error occurred during the execution of {}", sol::to_string(result.status()), script_name);
      if (t == sol::type::string) {
        err = fmt::format("{}: {}", err, sol::stack::unqualified_get<sol::string_view>(L, result.stack_index()));
      }
      // replacing information of stack error into pfr
      const int target = result.stack_index();
      if (result.pop_count() > 0) {
        sol::stack::remove(L, target, result.pop_count());
      }
      sol::stack::push(L, err);
      const int top = lua_gettop(L);
      const int towards = top - target;
      if (towards != 0) {
        lua_rotate(L, top, towards);
      }
      return result;
    }
  }

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

  void SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val) {
    // Use registry prefix to avoid stack manipulation of globals
    std::string registry_key = "__gta_" + name;
    lua_pushstring(L, registry_key.c_str());
    lua_pushstring(L, val.c_str());
    lua_rawset(L, LUA_REGISTRYINDEX);
  }

  std::string GetInternalLuaVar(lua_State* L, const std::string& name) {
    // Use registry prefix to avoid stack manipulation of globals
    std::string registry_key = "__gta_" + name;
    lua_pushstring(L, registry_key.c_str());
    lua_rawget(L, LUA_REGISTRYINDEX);
    std::string result = luaL_optstring(L, -1, "");
    lua_pop(L, 1);

    return result;
  }
  
  std::string GetCurrentFile(lua_State* L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "S", &ar);
    std::string file = std::filesystem::path(ar.source).filename().string();

    return file;
  }

  int GetCurrentLine(lua_State* L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "l", &ar);

    return ar.currentline;
  }

  std::string GetScriptName(lua_State* L) {
    return GetInternalLuaVar(L, "script_name");
  }

  void SetScriptName(lua_State* L, const std::string& name) {
    SetInternalLuaVar(L, "script_name", name);
  }

  std::filesystem::path GetScriptPath(lua_State* L) {
    return GetInternalLuaVar(L, "script_dir");
  }
  void SetScriptPath(lua_State* L, const std::filesystem::path& path) {
    SetInternalLuaVar(L, "script_dir", path.string());
  }

  std::filesystem::path GetMainFile(lua_State* L) {
    return GetInternalLuaVar(L, "script_main_file");
  }
  void SetMainFile(lua_State* L, const std::filesystem::path& path) {
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

  sol::protected_function_result RunScriptFileSafe(sol::state& sol, const std::filesystem::path& path) {
    return sol.safe_script_file(path.string(), SafeScriptErrorHandler);
  }
}
