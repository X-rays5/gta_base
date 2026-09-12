//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::bindings::coro {
  /**
   * Register the waiting calls of the `thread` namespace, the script's view of minicoropp::this_coro.
   * Only the calls that mean something to a script are bound; see the note in the implementation for
   * what is left out and why.
   */
  void RegisterCoro(AngelScript::asIScriptEngine* engine);
}
