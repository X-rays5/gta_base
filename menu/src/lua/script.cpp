//
// Created by X-ray on 04/04/2026.
//

#include "script.hpp"
#include <stacktrace>
#include "lua_util.hpp"
#include "lib/logging.hpp"

namespace base::menu::lua {
  namespace {
    void SetLibs(sol::state& lua_state) {
      lua_state.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::coroutine,
        sol::lib::string,
        sol::lib::math,
        sol::lib::table,
        sol::lib::bit32);
    }

    void SetExHandler(sol::state& lua_state) {
      lua_state.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_exception, std::string_view description) -> int {
      std::string err_msg = description.data();
      if (maybe_exception.has_value())
        err_msg = fmt::format("{}\n{}", *maybe_exception->what(), err_msg);

      LOG_ERROR("lua exception: {}\n{}", err_msg, std::to_string(std::stacktrace::current(1)));

      return sol::stack::push(L, description);
    });
    }

    void SetFuncs(sol::state& lua_state) {
      auto log_table = SetupLuaLogging(lua_state);
      CreateReadOnlyTable(lua_state, "LOG", log_table);
    }
  }

  Script::Script(const std::string& script_path) : meta_data_{script_path} {
    const auto status = meta_data_.ReadMetaData();
    if (!status) {
      LOG_ERROR("Failed to read script meta data: {}", status.error());
      return;
    }

    SetLibs(lua_state_);
    SetExHandler(lua_state_);
    SetFuncs(lua_state_);

    SetMainFile(lua_state_, meta_data_.GetMainFile());
    SetScriptPath(lua_state_, script_path);
    SetScriptName(lua_state_, meta_data_.GetName());
  }

  Script::~Script() {

  }

  void Script::Start() {
    const auto script_file = absolute(meta_data_.GetMainFile()).string();
    LOG_INFO("Starting script: {} ({})", meta_data_.GetName(), script_file);

    const auto res = lua_state_.safe_script_file(script_file);
    if (!res.valid()) {
      const sol::error err = res;
      LOG_ERROR("Failed to execute script: {}", err.what());
    }
  }
}
