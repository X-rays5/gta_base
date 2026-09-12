//
// Created by X-ray on 12/09/2026.
//

#pragma once
#include <future>
#include <memory>
#include <minicoropp/coroutine.hpp>
#include <stdexcept>

namespace base::menu::script {
  /**
   * One unit of work queued onto a TaskExecutor: a callable run as a coroutine, so that a body can
   * suspend itself from inside and be resumed by the executor's next tick without blocking the
   * thread it is on.
   *
   * The promise is shared rather than held by value because the coroutine's frame captures the last
   * reference to it: a body that runs to its end settles the future the queueing call handed out,
   * from inside the coroutine, whether it finished on the first tick or the hundredth.
   *
   * Resuming is single-threaded by contract. minicoropp does not support resuming one coroutine from
   * two threads, so a task belongs to the executor that queued it and is ticked only from that
   * executor's thread - the game executor from the game thread, the general one from the general
   * thread - and never migrates between them.
   */
  class Task {
  public:
    template <typename F>
    explicit Task(F&& cb) {
      promise_ = std::make_shared<std::promise<void>>();
      coro_ = std::make_unique<minicoropp::Coroutine>([this, callback = std::forward<F>(cb)]() mutable {
        callback();
        done_ = true;
        promise_->set_value();
      });
    }

    ~Task() = default;

    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;

    [[nodiscard]] bool IsDone() const;

    /**
     * The future is settled when the body returns, or carries the exception when it throws. It can
     * only be asked for once, which is what the executor does as it queues the task.
     */
    [[nodiscard]] std::future<void> GetFuture() const;

    /// Resumes the body once, and marks the task done if that was its last turn or if it threw.
    void Tick();

  private:
    bool done_{false};
    std::shared_ptr<std::promise<void>> promise_;
    std::unique_ptr<minicoropp::Coroutine> coro_;
  };
}
