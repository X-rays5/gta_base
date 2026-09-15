//
// Created by X-ray on 12/09/2026.
//

#include "as_lifecycle.hpp"

#include "../script/as_script_context.hpp"
#include "../script/as_script_manager.hpp"
#include "../util/as_bind.hpp"

#include <string>

// The script-facing `script` namespace, which is where a script's own lifetime is spelled out.
//
// unload() takes no argument, and that is the whole reason it can only ever unload the caller: the
// script is identified by the module it is running in, which Script builds under the manifest name -
// the same string the manager keeps it under. Nothing has to hand the script its own name, and no
// script can name another.
//
// The suspension is what makes the unload take effect immediately rather than at the end of the
// script's turn. Suspend() only flags the context; the VM honours it when this native returns, so
// Execute() comes back instead of running whatever followed the call, and the manager is free to let
// go of the script.
//
// Which run it reaches depends on who is calling: GameInit, GameTick and init() are all coroutines, so
// in any of them there is a context to suspend and the script stops where it stands, while a page's UI
// callback is a plain call with no coroutine of its own to park and runs on to its end. Either way the
// request itself is the manager's, and that is what stops the script being resumed again.
namespace base::menu::as::bindings::lifecycle {
  namespace {
    using script::ScriptContext;
    using script::ScriptManager;

    void Unload() {
      // The manager resolves this the same way the script name was chosen when the module was built,
      // which is what makes the lookup below the caller's own script and no other.
      const std::string name = ScriptManager::RunningScriptName();
      if (name.empty()) {
        LOG_ERROR("[AS] script::unload() called outside of a script, which has no effect");
        return;
      }

      if (!script::kAS_SCRIPT_MANAGER) {
        LOG_ERROR("[AS] script::unload() called with no script manager, which has no effect");
        return;
      }

      // Nothing may follow this in the script's own frame: Suspend() only records the wait, and the
      // VM acts on it once this native has returned.
      if (ScriptContext* const context = ScriptContext::Current()) {
        context->Suspend(ScriptContext::Wait::kIndefinitely);
      }

      if (const auto status = script::kAS_SCRIPT_MANAGER->UnloadScript(name); status.has_error()) {
        LOG_INFO("[AS] Script '{}' failed to unload itself: {}", name, status.error());
      } else {
        LOG_INFO("[AS] Script '{}' unloaded itself", name);
      }
    }
  }

  void RegisterLifecycle(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("script");

    util::RegisterGlobalFunction(engine, "void unload()", AngelScript::asFUNCTION(Unload), AngelScript::asCALL_CDECL)
      .Desc("Unloads the script that calls it, which is the only script it can unload. Everything "
            "after the call in this tick is skipped, and the script is gone by the next one - the "
            "engine and its module are released once the game thread is done with the tick it was "
            "running in.")
      .Returns("Nothing. It does not return to the caller if the script is running as a coroutine.");

    engine->SetDefaultNamespace("");
  }
}
