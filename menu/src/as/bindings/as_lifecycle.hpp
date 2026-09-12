//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::bindings::lifecycle {
  /**
   * Register the `script` namespace, the script's view of its own lifetime. It holds one call,
   * script::unload(), which is the only way a script can end itself.
   */
  void RegisterLifecycle(AngelScript::asIScriptEngine* engine);
}
