//
// Created by X-ray on 12/09/2026.
//

#include "as_exception.hpp"

namespace base::menu::as::util {
  AngelScriptException::AngelScriptException(AngelScript::asIScriptContext *ctx) {
    if (!ctx) {
      exception_msg_ = "No active context";
      function_name_ = "N/A";
      script_name_ = "N/A";
      file_name_ = "N/A";
      line_number_ = -1;
      return;
    }

    exception_msg_ = ctx->GetExceptionString();
    const AngelScript::asIScriptFunction *func = ctx->GetExceptionFunction();
    if (func) {
      function_name_ = func->GetName();
      const AngelScript::asIScriptModule *module = func->GetModule();
      if (module) {
        script_name_ = module->GetName();
      } else {
        script_name_ = "N/A";
      }
    } else {
      function_name_ = "N/A";
      script_name_ = "N/A";
    }
    file_name_ = "N/A";
    line_number_ = ctx->GetExceptionLineNumber();
  }
}