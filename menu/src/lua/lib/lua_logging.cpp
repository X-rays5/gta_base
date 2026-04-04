//
// Created by X-ray on 04/04/2026.
//

#include "lua_logging.hpp"
#include <fmt/args.h>
#include "../lua_util.hpp"

namespace base::menu::lua {
  namespace {
    std::string FormatLuaVariadicArgs(const std::string& format, sol::variadic_args va) {
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

  sol::table SetupLuaLogging(sol::state& L) {
    auto table = L.create_table_with();

    table.set_function("info", [](const std::string& msg, const sol::variadic_args& va) {
      LOG_INFO(FormatLuaVariadicArgs(msg, va));
    });

    table.set_function("warn", [](const std::string& msg, const sol::variadic_args& va) {
      LOG_WARN(FormatLuaVariadicArgs(msg, va));
    });

    table.set_function("error", [](const std::string& msg, const sol::variadic_args& va) {
      LOG_ERROR(FormatLuaVariadicArgs(msg, va));
    });

    table.set_function("debug", [](const std::string& msg, const sol::variadic_args& va) {
      LOG_DEBUG(FormatLuaVariadicArgs(msg, va));
    });

    table.set_function("format", [](const std::string& msg, const sol::variadic_args& va) {
      return FormatLuaVariadicArgs(msg, va);
    });

    return table;
  }
}