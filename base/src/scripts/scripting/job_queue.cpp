//
// Created by X-ray on 09/17/22.
//

#include "job_queue.hpp"

namespace gta_base::scripts {
  void JobQueue::RunTick() {
    std::unique_lock lock(mtx_);

    if (jobs_.empty())
      return;

    while (auto front = jobs_.front()) {
      std::invoke(front);

      jobs_.pop();
      if (jobs_.empty())
        break;
    }
  }
}