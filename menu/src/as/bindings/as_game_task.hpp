//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::bindings::game_task {
  /**
   * Register `thread::queue_game_task` and `thread::queue_general_task`, the script's ways onto the two
   * task executors. They are registered in the `thread` namespace, since a task body waits with the
   * same thread::yield/sleep/suspend as a GameTick, but live in their own file because what is behind
   * them is not minicoropp::this_coro, which is what as_coro.cpp binds.
   */
  void RegisterGameTask(AngelScript::asIScriptEngine* engine);
}
