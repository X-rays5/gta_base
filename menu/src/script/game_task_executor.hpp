//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include "task_executor.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::script {
  /**
   * The task queue of the game thread: what `thread::queue_game_task()` hands over ends up here.
   *
   * Its thread is the game's own - the game scripts' host ticks this executor every frame - so it has
   * no thread and no signal to park on, it is simply ticked. That is also why a task of this one is
   * allowed to touch the game: it runs on the thread the game expects to be called from.
   */
  class GameTaskExecutor final : public TaskExecutor {
  public:
    GameTaskExecutor();
    ~GameTaskExecutor() override;

    Type GetType() override {
      return Type::GameScript;
    }

    // QueueTask is inherited: the game thread is the one that runs them, and it is ticked by the game
    // rather than by this executor, so nothing has to be told that work arrived.
  };
  inline GameTaskExecutor* kGAME_TASK_EXECUTOR{};
}
