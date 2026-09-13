//
// Created by X-ray on 12/09/2026.
//

#ifndef AS_NOTIFY_HPP_3E7B41D0A5C28F96
#define AS_NOTIFY_HPP_3E7B41D0A5C28F96
#include <angelscript.h>

namespace base::menu::as::bindings::notify {
  /**
   * Binds the `notify` namespace: the notifications the menu shows a player, which a script raises as
   * `notify::info(...)`, `notify::warning(...)`, `notify::error(...)` or `notify::show(...)`.
   *
   * A top-level namespace rather than `gui::notify`, because a notification is not a page: a script that
   * only wants to tell the player something has no business holding a component to do it with.
   */
  void RegisterNotify(AngelScript::asIScriptEngine* engine);
}

#endif //AS_NOTIFY_HPP_3E7B41D0A5C28F96
