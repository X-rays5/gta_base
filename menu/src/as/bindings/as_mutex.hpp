//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::bindings::mutex {
  /**
   * Register minicoropp::Mutex as the script's `std::mutex`, alongside the other standard types the
   * add-ons register in the `std` namespace.
   */
  void RegisterMutex(AngelScript::asIScriptEngine* engine);
}
