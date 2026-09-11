//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>
#include <string>

namespace base::menu::as::util {
  std::string ArgToString(AngelScript::asIScriptGeneric* gen, int argIdx, int typeId);

  void RegisterAddOns(AngelScript::asIScriptEngine* engine);
}