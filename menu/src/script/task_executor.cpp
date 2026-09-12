//
// Created by X-ray on 12/09/2026.
//

#include "task_executor.hpp"
#include <algorithm>

namespace base::menu::script {
  bool TaskExecutor::HasPendingTasks() {
    common::concurrency::ScopedSpinlock lock(tasks_lock_);
    return !tasks_.empty();
  }

  void TaskExecutor::OnTick() {
    // Under the lock, the pass only picks up what is there when it starts. Resuming happens outside
    // it: a task is free to queue another task - that is the point of a queue that anything may push
    // to - and doing that from inside the lock would have it wait on the executor's own pass.
    std::vector<Task*> pass;
    {
      common::concurrency::ScopedSpinlock lock(tasks_lock_);
      pass.reserve(tasks_.size());
      for (const auto& task : tasks_) {
        pass.emplace_back(task.get());
      }
    }

    for (const auto task : pass) {
      task->Tick();
    }

    // The finished ones go, also under the lock. Tasks queued during the pass were never resumed by
    // it, so they are not done and stay for the next one.
    common::concurrency::ScopedSpinlock lock(tasks_lock_);
    tasks_.erase(std::ranges::remove_if(tasks_,
                                        [](const auto& task) {
                                          return task->IsDone();
                                        }).begin(),
                 tasks_.end());
  }
}
