//
// Created by X-ray on 12/09/2026.
//

#include "general_task_executor.hpp"

namespace base::menu::script {
  GeneralTaskExecutor::GeneralTaskExecutor(win32::Signal* wake_signal)
    : TaskExecutor("GeneralTaskExecutor"), wake_signal_(wake_signal) {
    kGENERAL_TASK_EXECUTOR = this;
  }

  GeneralTaskExecutor::~GeneralTaskExecutor() {
    kGENERAL_TASK_EXECUTOR = nullptr;
  }

  void GeneralTaskExecutor::NotifyQueued() {
    // Null when an executor is built without a loop to wake - a test driving the queue by hand, for
    // instance - in which case there is no one parked and nothing to do.
    if (wake_signal_) {
      wake_signal_->Notify();
    }
  }
}
