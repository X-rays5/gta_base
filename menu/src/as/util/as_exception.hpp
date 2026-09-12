//
// Created by X-ray on 12/09/2026.
//

#pragma once
#include <angelscript.h>

namespace base::menu::as::util {
  class AngelScriptException {
  public:
    explicit AngelScriptException(AngelScript::asIScriptContext *ctx);

    [[nodiscard]] std::string exception_msg() const {
      return exception_msg_;
    }

    [[nodiscard]] std::string function_name() const {
      return function_name_;
    }

    [[nodiscard]] std::string script_name() const {
      return script_name_;
    }

    [[nodiscard]] std::string file_name() const {
      return file_name_;
    }

    [[nodiscard]] int line_number() const {
      return line_number_;
    }

  private:
    std::string exception_msg_;
    std::string function_name_;
    std::string script_name_;
    std::string file_name_;
    int line_number_;
  };
}