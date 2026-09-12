//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <memory>
#include <string>

#include <spdlog/common.h>
#include <spdlog/logger.h>

namespace base::menu::as::util {
  /**
   * The logger one script instance logs through, registered under the script's own name, writing to
   * the console and to logs/scripts/<script name>/<script name>.log.
   *
   * It is asynchronous, and the location a line is logged from is an AngelScript one - the script's
   * name, the .as file and the line - rather than the C++ file the binding happens to live in. Copying
   * that location out of the engine's module before the line is queued is what makes the two
   * compatible; see the note on ScriptLoggers in the .cpp.
   */
  [[nodiscard]] std::shared_ptr<spdlog::logger> CreateScriptLogger(const std::string& script_name);

  /**
   * Takes a script's logger out of spdlog's registry, on the script's way out. The entry is dropped
   * only if it is still this logger that holds the name: an instance that was replaced by a later
   * load of the same script must not take the replacement's entry with it.
   *
   * Dropping it does not destroy the logger - the async queue holds a reference to it for as long as
   * it has lines of its own left to write, which is what lets a script unload immediately after
   * logging without losing, or corrupting, that last line.
   */
  void DestroyScriptLogger(const std::shared_ptr<spdlog::logger>& logger);

  /**
   * Writes one line, at `level`, from the script running on this thread - GameInit, a task, whichever
   * it is. The line carries the script's own location, so a caller has nothing to add to the message.
   *
   * A thread with no script running through it logs to the menu's logger instead, without a location:
   * there is no script to attribute the line to, and the module's section names belong to a module
   * the menu's logger has no hold on.
   */
  void LogFromScript(spdlog::level::level_enum level, const std::string& message);
}
