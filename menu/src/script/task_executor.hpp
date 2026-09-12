//
// Created by X-ray on 12/09/2026.
//

#pragma once
#include <base-common/concurrency/spinlock.hpp>

#include <future>
#include <memory>
#include <utility>
#include <vector>

#include "script_base.hpp"
#include "task.hpp"

namespace base::menu::script {
  /**
   * A queue of tasks for one thread to tick: what is queued from anywhere is run from the thread that
   * drives this script, which is what makes QueueTask safe to call from a thread that is not the one
   * the work belongs on.
   *
   * A subclass only has to say which type it is and, if its thread is parked while the queue is
   * empty, what to wake (see NotifyQueued).
   */
  class TaskExecutor : public ScriptBase {
  public:
    /**
     * Queues `cb` to be run by this executor's next tick, and returns the future its body settles.
     *
     * The task is created here, on the caller's thread, and resumed only on the executor's, which is
     * what it means for the callables the script layer queues - they are free to be long, to wait,
     * and to be handed over from a thread that cannot afford to run them.
     */
    template <typename F>
    std::future<void> QueueTask(F&& cb) {
      auto task = std::make_unique<Task>(std::forward<F>(cb));
      auto future = task->GetFuture();

      {
        common::concurrency::ScopedSpinlock lock(tasks_lock_);
        tasks_.emplace_back(std::move(task));
      }

      // Outside the lock: a wake-up is cheap, but it can also hand control to the executor's thread,
      // and that thread is going to want this lock.
      NotifyQueued();
      return future;
    }

    /// Whether a queued task has yet to finish. A task parked on a wait of its own counts as pending.
    [[nodiscard]] bool HasPendingTasks();

  protected:
    explicit TaskExecutor(const std::string& name) : ScriptBase(name) {}

    /// Called after a task is queued, from whichever thread queued it, and never while the queue is
    /// locked. Nothing to do by default: the game thread is ticked by the game and does not need
    /// telling. An executor with a thread of its own parks that thread on a signal and wakes it here.
    virtual void NotifyQueued() {}

    // Tasks need no one-time setup of their own: the coroutine library wants no fiber bookkeeping for
    // a task, and an executor that has its own thread has already done everything that thread needs.
    void OnInit() override {}

    /// Resumes every queued task once and drops the ones that finished. This is the only place tasks
    /// are resumed or erased, so it must run on one thread - the one the executor's type is ticked
    /// from.
    void OnTick() override;

  private:
    std::vector<std::unique_ptr<Task>> tasks_;
    common::concurrency::Spinlock tasks_lock_;
  };
}
