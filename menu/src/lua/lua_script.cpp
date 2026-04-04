//
// Created by X-ray on 04/04/2026.
//

#include "lua_script.hpp"
#include <stacktrace>
#include "lua_util.hpp"
#include "../script/game_task_executor.hpp"
#include "lib/lua_coroutine.hpp"
#include "lib/lua_logging.hpp"
#include "../natives/natives_sol2.hpp"

namespace base::menu::lua {
  namespace {
    void SetLibs(sol::state& lua_state) {
      lua_state.open_libraries(
        sol::lib::base,
        sol::lib::package,
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

    void register_vector3(sol::state& lua) {
      auto vector3_type = lua.new_usertype<natives::Vector3>(
        "Vector3",
        sol::constructors<natives::Vector3(), natives::Vector3(float, float, float)>()
      );

      // Register members
      vector3_type["x"] = &natives::Vector3::x;
      vector3_type["y"] = &natives::Vector3::y;
      vector3_type["z"] = &natives::Vector3::z;

      // Optional: add methods for common operations
      vector3_type["length"] = [](const natives::Vector3& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
      };

      vector3_type["__add"] = [](const natives::Vector3& a, const natives::Vector3& b) {
        return natives::Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
      };

      vector3_type["__sub"] = [](const natives::Vector3& a, const natives::Vector3& b) {
        return natives::Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
      };

      vector3_type["__mul"] = [](const natives::Vector3& v, float scalar) {
        return natives::Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
      };
    }

    void SetFuncs(sol::state& lua_state) {
      auto log_table = SetupLuaLogging(lua_state);
      CreateReadOnlyTable(lua_state, "LOG", log_table);

      auto coro_table = SetupCoroutine(lua_state);
      CreateReadOnlyTable(lua_state, "coro", coro_table);

      // Register custom types before natives
      register_vector3(lua_state);

      auto natives_table = natives::register_natives(lua_state);
      CreateReadOnlyTable(lua_state, "natives", natives_table);
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

  std::future<void> Script::Start() {
    const auto script_file = absolute(meta_data_.GetMainFile()).string();
    LOG_INFO("Starting script: {} ({})", meta_data_.GetName(), script_file);

    return script::kGAME_TASK_EXECUTOR->QueueTask([this, script_file] {
      const auto res = RunScriptFileSafe(lua_state_, script_file);
      if (!res.valid()) {
        const sol::error err = res;
        LOG_ERROR(err.what());
      }
    });
  }
}
