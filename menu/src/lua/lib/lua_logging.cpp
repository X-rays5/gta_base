//
// Created by X-ray on 04/04/2026.
//

#include "lua_logging.hpp"
#include <fmt/args.h>
#include "../lua_util.hpp"

namespace base::menu::lua {
  namespace {
    FORCE_INLINE std::string FormatLuaVariadicArgs(const sol::state& L, const std::string& format, sol::variadic_args va) {
      static std::string base_msg = "[{}:{}:{}] {}";

      if (!va.size())
        return fmt::format(fmt::runtime(base_msg), GetScriptName(L), GetCurrentFile(L), GetCurrentLine(L), format);

      fmt::dynamic_format_arg_store<fmt::format_context> ds;
      ds.reserve(va.size(), 0);

      for (auto&& arg : va) {
        ds.push_back(StackValueToString(arg.lua_state(), arg.stack_index()));
      }

      return fmt::format(fmt::runtime(base_msg), GetScriptName(L), GetCurrentFile(L), GetCurrentLine(L), fmt::vformat(format, ds));
    }
  }

  sol::table SetupLuaLogging(sol::state& L) {
    auto table = L.create_table_with();

    table.set_function("info", [&L](const std::string& msg, const sol::variadic_args& va) {
      LOG_INFO(FormatLuaVariadicArgs(L, msg, va));
    });

    table.set_function("warn", [&L](const std::string& msg, const sol::variadic_args& va) {
      LOG_WARN(FormatLuaVariadicArgs(L, msg, va));
    });

    table.set_function("error", [&L](const std::string& msg, const sol::variadic_args& va) {
      LOG_ERROR(FormatLuaVariadicArgs(L, msg, va));
    });

    table.set_function("debug", [&L](const std::string& msg, const sol::variadic_args& va) {
      LOG_DEBUG(FormatLuaVariadicArgs(L, msg, va));
    });

    table.set_function("format", [&L](const std::string& msg, const sol::variadic_args& va) {
      return FormatLuaVariadicArgs(L, msg, va);
    });

    return table;
  }
}