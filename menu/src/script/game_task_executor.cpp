//
// Created by X-ray on 20/12/2025.
//

#include "game_task_executor.hpp"
#include <algorithm>
#include "../natives/natives_gen9.hpp"

namespace base::menu::script {
  GameTaskExecutor::GameTaskExecutor() : ScriptBase("GameTaskExecutor") {
    kGAME_TASK_EXECUTOR = this;
  }

  GameTaskExecutor::~GameTaskExecutor() {
    kGAME_TASK_EXECUTOR = nullptr;
  }

  void GameTaskExecutor::OnInit() {
    // Coroutines do not require OS-level fiber setup on the main thread
  }

  void GameTaskExecutor::OnTick() {
    for (const auto& task : tasks_) {
      task->Tick();
    }

    tasks_.erase(std::ranges::remove_if(tasks_,
                                        [](const auto& task) {
                                          return task->IsDone();
                                        }).begin(),
                 tasks_.end());
  }

  void GameTaskExecutor::GameTask::Tick() {
    if (done_)
      return;

    const auto res = coro_->resume();
    if (res != common::coroutine::CoroResult::kSUCCESS && res != common::coroutine::CoroResult::kYIELDING) {
      // If the coroutine threw an exception, capture it and mark as done
      if (res == common::coroutine::CoroResult::kEXCEPTION) {
        promise_->set_exception(coro_->exception());
      } else {
        // For any other unexpected result, set a generic exception
        promise_->set_exception(std::make_exception_ptr(std::runtime_error("Coroutine failed with unexpected result")));
      }
      done_ = true;
    }
  }

  bool GameTaskExecutor::GameTask::IsDone() const {
    return done_;
  }

  std::future<void> GameTaskExecutor::GameTask::GetFuture() const {
    return promise_->get_future();
  }
}
