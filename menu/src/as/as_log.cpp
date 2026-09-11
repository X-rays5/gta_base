//
// Created by X-ray on 11/09/2026.
//

#include "as_log.hpp"
#include "as_bind.hpp"
#include "as_util.hpp"

#include <fmt/args.h>

namespace base::menu::as::log {
  namespace {
    std::string FormatASVariadicArgs(AngelScript::asIScriptGeneric* gen) {
      static std::string base_msg = "[{}:{}:{}] {}";

      std::string script_file = "unknown";
      int line_number = 0;
      std::string script_name = "unknown";

      AngelScript::asIScriptContext* ctx = AngelScript::asGetActiveContext();
      if (ctx) {
        const char* section_name = nullptr;
        line_number = ctx->GetLineNumber(0, nullptr, &section_name);
        if (section_name) {
          script_file = section_name;
        }

        const AngelScript::asIScriptFunction* func = ctx->GetFunction(0);
        if (func && func->GetModule()) {
          script_name = func->GetModule()->GetName();
        }
      }

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

      std::string formatted_payload;
      if (ds.size() > 0) {
        try {
          formatted_payload = fmt::vformat(format, ds);
        } catch (const std::exception& e) {
          formatted_payload = fmt::format("{} [Format Error: {}]", format, e.what());
        }
      } else {
        formatted_payload = format;
      }

      return fmt::format(fmt::runtime(base_msg), script_name, script_file, line_number, formatted_payload);
    }

    void LogInfoGeneric(AngelScript::asIScriptGeneric* gen)  { LOG_INFO(FormatASVariadicArgs(gen)); }
    void LogWarnGeneric(AngelScript::asIScriptGeneric* gen)  { LOG_WARN(FormatASVariadicArgs(gen)); }
    void LogErrorGeneric(AngelScript::asIScriptGeneric* gen) { LOG_ERROR(FormatASVariadicArgs(gen)); }
    void LogDebugGeneric(AngelScript::asIScriptGeneric* gen) { LOG_DEBUG(FormatASVariadicArgs(gen)); }

    void FormatGeneric(AngelScript::asIScriptGeneric* gen) {
      std::string result = FormatASVariadicArgs(gen);

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

    // info
    RegisterGlobalFunction(engine, "void info(const std::string &in)", asFUNCTION(LogInfoGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs an informational message to the menu console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    RegisterGlobalFunction(engine, "void info(const std::string &in, const ?&in...)", asFUNCTION(LogInfoGeneric), AngelScript::asCALL_GENERIC);

    // warn
    RegisterGlobalFunction(engine, "void warn(const std::string &in)", asFUNCTION(LogWarnGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs a warning to the menu console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    RegisterGlobalFunction(engine, "void warn(const std::string &in, const ?&in...)", asFUNCTION(LogWarnGeneric), AngelScript::asCALL_GENERIC);

    // error
    RegisterGlobalFunction(engine, "void error(const std::string &in)", asFUNCTION(LogErrorGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs an error to the menu console.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    RegisterGlobalFunction(engine, "void error(const std::string &in, const ?&in...)", asFUNCTION(LogErrorGeneric), AngelScript::asCALL_GENERIC);

    // debug
    RegisterGlobalFunction(engine, "void debug(const std::string &in)", asFUNCTION(LogDebugGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Logs a debug message to the menu console, which is only written when debug logging is enabled.")
      .Param("in", "The message to log. Format placeholders are replaced by the trailing arguments.");
    RegisterGlobalFunction(engine, "void debug(const std::string &in, const ?&in...)", asFUNCTION(LogDebugGeneric), AngelScript::asCALL_GENERIC);

    // format
    RegisterGlobalFunction(engine, "std::string format(const std::string &in)", asFUNCTION(FormatGeneric), AngelScript::asCALL_GENERIC)
      .Desc("Formats a message the same way the log functions do, without writing it to the console.")
      .Param("in", "The format string. Placeholders are replaced by the trailing arguments.")
      .Returns("The formatted message.");
    RegisterGlobalFunction(engine, "std::string format(const std::string &in, const ?&in...)", asFUNCTION(FormatGeneric), AngelScript::asCALL_GENERIC);

    engine->SetDefaultNamespace("");
  }
}
