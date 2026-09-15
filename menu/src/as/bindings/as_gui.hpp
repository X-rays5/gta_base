//
// Created by X-ray on 12/09/2026.
//

#ifndef AS_GUI_HPP_6F1C0A24E9B37D58
#define AS_GUI_HPP_6F1C0A24E9B37D58
#include <angelscript.h>

namespace base::menu::as::bindings::gui {
  /**
   * Binds the `gui` namespace: the components a script draws a page out of, the page itself, and the
   * translations a script adds under `gui::translations`.
   *
   * Must be called *after* `RegisterOption`, and not before it: `gui::Submenu::AddOption` names
   * `Option`, and `SetUiCallback` - which is registered here rather than in the option binding, since
   * the funcdef it takes only exists once this has run - is registered onto it. The option binding
   * knows nothing about the GUI and stays that way; what it would have to be reordered around is the
   * one thing here that names it.
   */
  void RegisterGui(AngelScript::asIScriptEngine* engine);
}

#endif //AS_GUI_HPP_6F1C0A24E9B37D58
