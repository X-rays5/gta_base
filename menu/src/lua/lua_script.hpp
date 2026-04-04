//
// Created by X-ray on 04/04/2026.
//

#pragma once

#pragma warning(push)
#pragma warning(disable : 5321)
#include <sol/sol.hpp>
#pragma warning(pop)
#include <future>

#include "lua_script_metadata.hpp"

namespace base::menu::lua {
  class Script {
  public:
    explicit Script(const std::string& script_path);
    ~Script();

    Script(const Script&) = delete;
    Script(Script&&) = delete;
    Script& operator=(const Script&) = delete;
    Script& operator=(Script&&) = delete;

    std::future<void> Start();

    sol::state& GetLuaState() { return lua_state_; }

  private:
    ScriptMetaData meta_data_;
    sol::state lua_state_;

  private:
    //int ExceptionHandler(lua_State* l, sol::optional<const std::exception&> ex, std::string_view msg);
  };
}