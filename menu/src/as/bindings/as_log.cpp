//
// Created by X-ray on 11/09/2026.
//

#include "as_log.hpp"
#include "../util/as_bind.hpp"
#include "../util/as_script_logger.hpp"
#include "../util/as_util.hpp"

#include <fmt/args.h>

namespace base::menu::as::bindings::log {
  namespace {
    std::string FormatASPayload(AngelScript::asIScriptGeneric* gen) {
      // Format string is argument 0
      const auto* fmt_ptr = static_cast<std::string*>(gen->GetArgAddress(0));
      const std::string format = fmt_ptr ? *fmt_ptr : "";

      // totalArgs is the actual number of parameters passed (Format string = 0, Varargs = 1..totalArgs-1)
      const int totalArgs = gen->GetArgCount();
      fmt::dynamic_format_arg_store<fmt::format_context> ds;

      for (int i = 1; i < totalArgs; ++i) {
        const int typeId = gen->GetArgTypeId(i);
        ds.push_back(util::ArgToString(gen, i, typeId));
      }

      if (ds.size() > 0) {
        try {
          return fmt::vformat(format, ds);
        } catch (const std::exception& e) {
          return fmt::format("{} [Format Error: {}]", format, e.what());
        }
      }

      return format;
    }

    // The script and the line around each of these are the logger's business, not the message's:
    // the logger is the script's own, and it is what records where in the script the line came from.
    void LogInfoGeneric(AngelScript::asIScriptGeneric* gen)  { util::LogFromScript(spdlog::level::info, FormatASPayload(gen)); }
    void LogWarnGeneric(AngelScript::asIScriptGeneric* gen)  { util::LogFromScript(spdlog::level::warn, FormatASPayload(gen)); }
    void LogErrorGeneric(AngelScript::asIScriptGeneric* gen) { util::LogFromScript(spdlog::level::err, FormatASPayload(gen)); }

    // log::debug is a no-op in a release build, as LOG_DEBUG is, and the message is not built at
    // all - which is why this one carries the guard the other three do not need.
    void LogDebugGeneric(AngelScript::asIScriptGeneric* gen) {
#ifndef NDEBUG
      util::LogFromScript(spdlog::level::debug, FormatASPayload(gen));
#else
      static_cast<void>(gen);
#endif
    }

    void FormatGeneric(AngelScript::asIScriptGeneric* gen) {
      std::string result = FormatASPayload(gen);

      auto* retPtr = static_cast<std::string*>(gen->GetAddressOfReturnLocation());
      if (retPtr) {
        // Construct std::string in-place in AngelScript's uninitialized return buffer
        new (retPtr) std::string(std::move(result));
      }
    }
  }

  void RegisterLog(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("log");

    // Declarations are resolved relative to the default namespace, and 'log' is a
    // sibling of 'std' rather than a child of it, so 'string' has to be qualified.

    // Docs are keyed by name, so only one overload of each function carries them.

    // Each of these writes to the script's own log, logs/scripts/<script name>/<script name>.log,
    // and to the console, and every line records the script file and line it was logged from.

    // info
    util::RegisterGlobalFunction(engine, "void info(const std::string &in)", asFUNCTION(LogInfoGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs an informational message to the script's own log file and to the console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    util::RegisterGlobalFunction(engine, "void info(const std::string &in, const ?&in...)", asFUNCTION(LogInfoGeneric), AngelScript::asCALL_GENERIC);

    // warn
    util::RegisterGlobalFunction(engine, "void warn(const std::string &in)", asFUNCTION(LogWarnGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs a warning to the script's own log file and to the console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    util::RegisterGlobalFunction(engine, "void warn(const std::string &in, const ?&in...)", asFUNCTION(LogWarnGeneric), AngelScript::asCALL_GENERIC);

    // error
    util::RegisterGlobalFunction(engine, "void error(const std::string &in)", asFUNCTION(LogErrorGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs an error to the script's own log file and to the console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    util::RegisterGlobalFunction(engine, "void error(const std::string &in, const ?&in...)", asFUNCTION(LogErrorGeneric), AngelScript::asCALL_GENERIC);

    // debug
    util::RegisterGlobalFunction(engine, "void debug(const std::string &in)", asFUNCTION(LogDebugGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs a debug message to the script's own log file and to the console, which is only written when debug logging is enabled.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    util::RegisterGlobalFunction(engine, "void debug(const std::string &in, const ?&in...)", asFUNCTION(LogDebugGeneric), AngelScript::asCALL_GENERIC);

    // format
    util::RegisterGlobalFunction(engine, "std::string format(const std::string &in)", asFUNCTION(FormatGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Formats a message the same way the log functions do, without writing it anywhere.")
      .Param("in", "The format string. Placeholders are replaced by the trailing arguments.")
      .Returns("The formatted message.");
    util::RegisterGlobalFunction(engine, "std::string format(const std::string &in, const ?&in...)", asFUNCTION(FormatGeneric), AngelScript::asCALL_GENERIC);

    engine->SetDefaultNamespace("");
  }
}
