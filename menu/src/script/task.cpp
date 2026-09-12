//
// Created by X-ray on 12/09/2026.
//

#include "task.hpp"

namespace base::menu::script {
  void Task::Tick() {
    if (done_) {
      return;
    }

    const auto res = coro_->resume();
    if (res != minicoropp::CoroResult::kSUCCESS && res != minicoropp::CoroResult::kYIELDING) {
      // A body that threw carries its exception to whoever is waiting on the future rather than
      // taking the thread down with it. Anything else is a result the coroutine library has no
      // spelling for here, so it is reported as the failure it is.
      if (res == minicoropp::CoroResult::kEXCEPTION) {
        promise_->set_exception(coro_->exception());
      } else {
        promise_->set_exception(std::make_exception_ptr(std::runtime_error("Coroutine failed with unexpected result")));
      }

      done_ = true;
    }
  }

  bool Task::IsDone() const {
    return done_;
  }

  std::future<void> Task::GetFuture() const {
    return promise_->get_future();
  }
}
