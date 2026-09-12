//
// Created by X-ray on 12/09/2026.
//

#pragma once
#include <base-common/win32/signal.hpp>

#include "task_executor.hpp"

namespace base::menu::script {
  /**
   * The task queue of the general thread: what `thread::queue_general_task()` hands over ends up here.
   *
   * Unlike the game executor, this one owns the thread that ticks it, and that thread has nothing
   * else to do - so it must not spin while the queue is empty. It parks on a signal instead, and
   * this executor is the thing that rings it: every queued task notifies, so a task queued onto an
   * idle general thread is picked up at once rather than at the next tick of a cadence.
   *
   * The signal belongs to whoever runs the loop (the ScriptManager) - this executor only rings it.
   */
  class GeneralTaskExecutor final : public TaskExecutor {
  public:
    explicit GeneralTaskExecutor(win32::Signal* wake_signal);
    ~GeneralTaskExecutor() override;

    Type GetType() override {
      return Type::General;
    }

    // QueueTask is inherited. It calls NotifyQueued, which is the whole reason this executor takes the
    // signal: the thread that runs the task is parked and has to be told there is one.
  protected:
    void NotifyQueued() override;

  private:
    win32::Signal* wake_signal_{};
  };
  inline GeneralTaskExecutor* kGENERAL_TASK_EXECUTOR{};
}
