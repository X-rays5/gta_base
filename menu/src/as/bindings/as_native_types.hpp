//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::bindings::native_types {
  /**
   * Register the types the generated native bindings are declared in terms of: Vector2/3/4, Hash and
   * NativeHandle. Must run before natives::RegisterAngelScript, which the declarations reference.
   */
  void Register(AngelScript::asIScriptEngine* engine);
}
