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
      LUA_LOG_ERROR(logger_, magic_enum::enum_name(script.status()));
      return;
    }
  }

  Script::~Script() {
    auto result = CallLuaFunction(lua_state_, "Shutdown");

    if (result.has_value() && !result->valid()) {
      LUA_LOG_WARN(logger_, "{} doesn't have a Shutdown function", GetInternalLuaVar("script_main_file"));

      PROTECTED_FUNCTION_RESULT_LOG_SAFE((*result))
    }
  }

  void Script::Init() {
    auto result = CallLuaFunction(lua_state_, "Init");

    if (result.has_value() && !result->valid()) {
      LOG_ERROR("{} doesn't have a Init function", GetInternalLuaVar("script_main_file"));

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

  void Script::SetInternalLuaVar(const std::string& name, const std::string& val) {
    lua_getglobal(lua_state_, "SCRIPT_INFO");
    lua_pushstring(lua_state_, val.c_str());
    lua_setfield(lua_state_, -2, name.c_str());
  }

  std::string Script::GetInternalLuaVar(const std::string& name) {
    lua_getglobal(lua_state_, "SCRIPT_INFO");
    lua_getfield(lua_state_, -1, name.c_str());

    return StackValueToString(lua_state_, -1);
  }

  std::string Script::GetCurrentFile() {
    lua_Debug ar;
    lua_getstack(lua_state_, 1, &ar);
    lua_getinfo(lua_state_, "S", &ar);
    std::string file = std::filesystem::path(ar.source).filename().string();

    return file;
  }
  int Script::GetCurrentLine() {
    lua_Debug ar;
    lua_getstack(lua_state_, 1, &ar);
    lua_getinfo(lua_state_, "l", &ar);

    return ar.currentline;
  }

  std::string Script::GetScriptName() {
    return GetInternalLuaVar("script_name");
  }

  void Script::SetScriptName(const std::string& name) {
    SetInternalLuaVar("script_name", name);
  }

  std::filesystem::path Script::GetScriptPath() {
    return GetInternalLuaVar("script_dir");
  }
  void Script::SetScriptPath(const std::filesystem::path& path) {
    SetInternalLuaVar("script_dir", path.string());
  }

  std::filesystem::path Script::GetMainFile() {
    return GetInternalLuaVar("script_main_file");
  }
  void Script::SetMainFile(const std::filesystem::path& path) {
    SetInternalLuaVar("script_main_file", path.string());
  }

  void Script::AddFunctions() {
    auto natives = lua_natives::CreateSol2Bindings(lua_state_);
    CreateReadOnlyTable("natives", natives);

    auto logging = CreateLoggingFunctions();
    CreateReadOnlyTable("log", logging);
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

  sol::table Script::CreateReadOnlyTable(const std::string& name, sol::table& meta_table) {
    auto table = lua_state_.create_named_table(name);
    meta_table[sol::meta_function::new_index] = ReadOnly;
    meta_table[sol::meta_function::index] = meta_table;

    table[sol::metatable_key] = meta_table;

    return table;
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

  int Script::ReadOnly(lua_State* L) {
    return luaL_error(L, "This table is read only");
  }
}

#undef PROTECTED_FUNCTION_RESULT_LOG_SAFE