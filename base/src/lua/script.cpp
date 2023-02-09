//
// Created by X-ray on 11/27/22.
//

#include <regex>
#include <fmt/args.h>
#include <stacktrace>
#include "script.hpp"
#include "manager.hpp"

#define PROTECTED_FUNCTION_RESULT_LOG_SAFE(result) if (lua_type(result.lua_state(), result.stack_index()) == LUA_TSTRING) {LOG_ERROR(result);}

namespace gta_base::lua {
  namespace {
    inline std::string FormatSolError(const sol::error& err) {
      //std::string result = err.what();
      //LOG_DEBUG(result);

      /*std::regex regex(R"(([A-Za-z0-9_\-\.]+\.[A-Za-z0-9]+:\d*): ?(.*))");
      std::smatch match;

      if (std::regex_search(result, match, regex)) {
        result = fmt::format("[{}] {}", match[1].str(), match[2].str());
      }*/

      // replace all \n with \r\n
      std::string err_str = err.what();
      for (std::size_t i = 0; i < err_str.size(); i++) {
        if (err_str[i] == '\n') {
          err_str.insert(i, "\r");
          i++;
        }
      }

      return fmt::format("\r\n{}", err_str);
    }
  }

  Script::Script(const std::string& name, const std::filesystem::path& script_dir, const std::filesystem::path& main_file) : logger_(&lua_state_, name) {
    LoadLibraries();
    SetExceptionHandler();
    AddFunctions();

    auto script_info_table = lua_state_.create_named_table("SCRIPT_INFO");

    SetScriptName(name);
    SetScriptPath(script_dir);
    SetMainFile(main_file);

    sol::protected_function_result script = lua_state_.safe_script_file(main_file.string());
    if (!script.valid()) {
      LUA_LOG_ERROR(logger_, FormatSolError(script));
      return;
    }
  }

  Script::~Script() {
    auto result = CallLuaFunction(lua_state_, "Shutdown");

    if (result.has_value() && !result->valid()) {
      LUA_LOG_WARN(logger_, "{} doesn't have a Shutdown function", GetInternalLuaVar(lua_state_, "script_main_file"));

      PROTECTED_FUNCTION_RESULT_LOG_SAFE((*result))
    }
  }

  void Script::Init() {
    auto result = CallLuaFunction(lua_state_, "Init");

    if (result.has_value() && !result->valid()) {
      LOG_ERROR("{} doesn't have a Init function", GetInternalLuaVar(lua_state_, "script_main_file"));

      PROTECTED_FUNCTION_RESULT_LOG_SAFE((*result))
    }
  }

  void Script::Tick() {
    if (first_tick_) {
      Init();
      first_tick_ = false;
    }

    if (!has_tick_func_)
      return;

    auto result = CallLuaFunction(lua_state_, "Tick");
    has_tick_func_ = result.has_value() && result->valid();
  }

  void Script::LoadLibraries() {
    lua_state_.open_libraries(
    sol::lib::base,
    sol::lib::package,
    sol::lib::coroutine,
    sol::lib::string,
    sol::lib::os,
    sol::lib::math,
    sol::lib::table,
    sol::lib::debug,
    sol::lib::bit32,
    sol::lib::io,
    sol::lib::utf8
    );
  }

  void Script::SetExceptionHandler() {
    lua_state_.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) -> int {
      std::string err_msg = description.data();
      if (maybe_exception.has_value())
        err_msg = fmt::format("{}\n{}", *maybe_exception->what(), err_msg);

      LOG_ERROR("lua exception: {}\n{}", err_msg, std::to_string(std::stacktrace::current(1)));

      kLOGGER->Flush();

      return sol::stack::push(L, description);
    });
  }

  FORCE_INLINE void Script::SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val) {
    lua_getglobal(L, "SCRIPT_INFO");
    lua_pushstring(L, val.c_str());
    lua_setfield(L, -2, name.c_str());
  }

  FORCE_INLINE std::string Script::GetInternalLuaVar(lua_State* L, const std::string& name) {
    lua_getglobal(L, "SCRIPT_INFO");
    lua_getfield(L, -1, name.c_str());

    return StackValueToString(L, -1);
  }

  void Script::AddFunctions() {
    auto natives = lua_natives::CreateSol2Bindings(lua_state_);
    CreateReadOnlyTable("natives", natives);

    auto logging = CreateLoggingFunctions();
    CreateReadOnlyTable("log", logging);
  }

  sol::table Script::CreateReadOnlyTable(const std::string& name, sol::table& meta_table) {
    auto table = lua_state_.create_named_table(name);
    meta_table[sol::meta_function::new_index] = ReadOnly;
    meta_table[sol::meta_function::index] = meta_table;

    table[sol::metatable_key] = meta_table;

    return table;
  }

  std::string Script::GetCurrentFile(lua_State* L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "S", &ar);
    std::string file = std::filesystem::path(ar.source).filename().string();

    return file;
  }

  int Script::GetCurrentLine(lua_State* L) {
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "l", &ar);

    return ar.currentline;
  }

  int Script::ReadOnly(lua_State* L) {
    return luaL_error(L, "This table is read only");
  }

  std::string Script::FormatLuaVariadicArgs(const std::string& format, sol::variadic_args va) {
    if (!va.size())
      return format;

    fmt::dynamic_format_arg_store<fmt::format_context> ds;
    ds.reserve(va.size(), 0);

    for (auto&& arg : va) {
      ds.push_back(StackValueToString(arg.lua_state(), arg.stack_index()));
    }

    return fmt::vformat(format, ds);
  }

  std::optional<sol::protected_function_result> Script::CallLuaFunction(sol::state& L, const std::string& fn_name) {
    try {
      return sol::protected_function(L[fn_name])();
    } catch (sol::error& e) {
      LUA_LOG_ERROR(logger_, "LUA: {}", e.what());
    } catch (std::exception& e) {
      LUA_LOG_ERROR(logger_, e.what());
    } catch (...) {
      LUA_LOG_ERROR(logger_, "Unknown exception occurred.");
    }

    return std::nullopt;
  }

  std::string Script::GetScriptName() {
    return GetInternalLuaVar(lua_state_, "script_name");
  }
  void Script::SetScriptName(const std::string& name) {
    SetInternalLuaVar(lua_state_, "script_name", name);
  }

  std::filesystem::path Script::GetScriptPath() {
    return GetInternalLuaVar(lua_state_, "script_dir");
  }
  void Script::SetScriptPath(const std::filesystem::path& path) {
    SetInternalLuaVar(lua_state_, "script_dir", path.string());
  }

  std::filesystem::path Script::GetMainFile() {
    return GetInternalLuaVar(lua_state_, "script_main_file");
  }
  void Script::SetMainFile(const std::filesystem::path& path) {
    SetInternalLuaVar(lua_state_, "script_main_file", path.string());
  }
  sol::table Script::CreateLoggingFunctions() {
    auto logging = lua_state_.create_table_with();

    logging.set_function("info", [&](const std::string& msg, sol::variadic_args va) {
      LUA_LOG_INFO(logger_, FormatLuaVariadicArgs(msg, va));
    });

    logging.set_function("warn", [&](const std::string& msg, sol::variadic_args va) {
      LUA_LOG_WARN(logger_, FormatLuaVariadicArgs(msg, va));
    });

    logging.set_function("error", [&](const std::string& msg, sol::variadic_args va) {
      LUA_LOG_ERROR(logger_, FormatLuaVariadicArgs(msg, va));
    });

    logging.set_function("critical", [&](const std::string& msg, sol::variadic_args va) {
      LUA_LOG_CRITICAL(logger_, FormatLuaVariadicArgs(msg, va));
    });

    logging.set_function("debug", [&](const std::string& msg, sol::variadic_args va) {
      #ifndef NDEBUG
      LUA_LOG_DEBUG(logger_, FormatLuaVariadicArgs(msg, va));
      #else
      LUA_LOG_WARN(logger_, "This function is meant only for development builds");
      #endif
    });

    logging.set_function("format", [&](const std::string& msg, sol::variadic_args va) {
      return FormatLuaVariadicArgs(msg, va);
    });

    return logging;
  }
}

#undef PROTECTED_FUNCTION_RESULT_LOG_SAFE