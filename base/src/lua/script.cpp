//
// Created by X-ray on 11/27/22.
//

#include <regex>
#include <fmt/args.h>
#include "script.hpp"
#include "manager.hpp"

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
        if(err_str[i] == '\n') {
          err_str.insert(i, "\r");
          i++;
        }
      }

      return fmt::format("\r\n{}", err_str);
    }
  }

  Script::Script(const std::filesystem::path& script_path) {
    LoadLibraries();
    SetExceptionHandler();
    AddFunctions();

    lua_state_.create_named_table("__internal");
    SetInternalLuaVar(lua_state_, "script_path", script_path.string());

    sol::protected_function_result script = lua_state_.safe_script_file(script_path.string());
    if (!script.valid()) {
      LOG_ERROR(FormatSolError(script));
      return;
    }
  }

  Script::~Script() {
    sol::protected_function_result shutdown_result = lua_state_["Shutdown"]();
    if (!shutdown_result.valid()) {
      LOG_WARN(FormatSolError(shutdown_result));
    } else {
      LOG_INFO("Successfully ran Shutdown func for {}", std::filesystem::path(GetInternalLuaVar(lua_state_.lua_state(), "script_path")).filename().string());
    }
  }

  void Script::Init() {
    sol::protected_function_result init_result = lua_state_["Init"]();
    if (!init_result.valid()) {
      LOG_WARN(FormatSolError(init_result));
    } else {
      LOG_INFO("Successfully ran Init func for {}", GetInternalLuaVar(lua_state_.lua_state(), "script_path"));
    }
  }

  void Script::Tick() {
    if (first_tick_) {
      Init();
      first_tick_ = false;
    }

    if (!has_tick_func_)
      return;

    auto res = lua_state_["Tick"]();
    has_tick_func_ = res.valid();
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
    lua_state_.set_panic(Panic);

    lua_state_.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) -> int {
      return sol::stack::push(L, description);
    });
  }

  int Script::Panic(lua_State* L) {
    LOG_ERROR("A lua panic occurred: {}", lua_tostring(L, -1));

    auto script_path = GetInternalLuaVar(L, "script_path");
    kMANAGER->RemoveScript(script_path);

    lua_close(L);

    return 0;
  }

  void Script::SetInternalLuaVar(lua_State* L, const std::string& name, const std::string& val) {
    lua_getglobal(L, "__internal");
    lua_pushstring(L, val.c_str());
    lua_setfield(L, -2, name.c_str());
  }

  std::string Script::GetInternalLuaVar(lua_State* L, const std::string& name) {
    lua_getglobal(L, "__internal");
    lua_getfield(L, -1, name.c_str());
    std::string val = lua_tostring(L, -1);

    return val;
  }


  void Script::AddFunctions() {
    auto logging_metatable = lua_state_.create_table_with();

    logging_metatable.set_function("info", [&](const std::string& msg, sol::variadic_args va) {
      LOG_INFO("[{}:{}] {}", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_), FormatLuaVariadicArgs(msg, va));
    });

    logging_metatable.set_function("warn", [&](const std::string& msg, sol::variadic_args va) {
      LOG_WARN("[{}:{}] {}", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_), FormatLuaVariadicArgs(msg, va));
    });

    logging_metatable.set_function("error", [&](const std::string& msg, sol::variadic_args va) {
      LOG_ERROR("[{}:{}] {}", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_), FormatLuaVariadicArgs(msg, va));
    });

    logging_metatable.set_function("fatal", [&](const std::string& msg, sol::variadic_args va) {
      LOG_FATAL("[{}:{}] {}", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_), FormatLuaVariadicArgs(msg, va));
    });

    logging_metatable.set_function("debug", [&](const std::string& msg, sol::variadic_args va) {
#ifndef NDEBUG
      LOG_DEBUG("[{}:{}] {}", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_), FormatLuaVariadicArgs(msg, va));
#else
      LOG_WARN("[{}:{}] This function is meanth only for development builds", GetCurrentFile(lua_state_), GetCurrentLine(lua_state_));
#endif
    });

    logging_metatable.set_function("format", [&](const std::string& msg, sol::variadic_args va) {
      return FormatLuaVariadicArgs(msg, va);
    });

    CreateReadOnlyTable("log", logging_metatable);
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
}