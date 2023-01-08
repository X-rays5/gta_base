//
// Created by X-ray on 08/12/22.
//

#include "pool.hpp"
#include "script.hpp"
#include "manager.hpp"

namespace gta_base::fiber {
  Pool::Pool(std::size_t num_per_tick) {
    if (num_per_tick == 0)
      num_per_tick = 1;

    for (int i = 0; i < num_per_tick; i++)
      kMANAGER->AddScript(std::make_unique<Script>(&FiberFunc));

    kPOOL = this;
  }

  Pool::~Pool() {
    kPOOL = nullptr;
  }

  void Pool::AddJob(const job_t& job) {
    if (job) {
      std::unique_lock lock(mtx_);
      jobs_.push(job);
    }
  }

  void Pool::Tick() {
    std::unique_lock lock(mtx_);
    if (!jobs_.empty()) {
      auto job = std::move(jobs_.front());
      jobs_.pop();
      lock.unlock();

      std::invoke(job);
    }
  }

  [[noreturn]] void Pool::FiberFunc() {
    while (true) {
      kPOOL->Tick();
      Script::GetCurr()->Yield();
    }
  }
}