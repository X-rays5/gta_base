//
// Created by X-ray on 09/17/22.
//

#pragma once
#ifndef GTA_BASE_JOB_QUEUE_HPP
#define GTA_BASE_JOB_QUEUE_HPP
#include <queue>
#include <functional>
#include <mutex>
#include "../../scriptmanager/base_script.hpp"

#undef AddJob

namespace gta_base::scripts {
  class JobQueue;

  inline JobQueue* kJOB_QUEUE{};

  class JobQueue : public scriptmanager::BaseScript {
  public:
    using job_t = std::function<void()>;

  public:
    JobQueue() {
      kJOB_QUEUE = this;
      initialized_ = true;
    }

    ~JobQueue() {
      kJOB_QUEUE = nullptr;
    }

    scriptmanager::ScriptType GetType() final {
      return scriptmanager::ScriptType::kScripting;
    }

    void AddJob(job_t job) {
      mtx_.lock();
      jobs_.push(std::move(job));
      mtx_.unlock();
    }

    void Init() final {};
    void RunTick() final;

  private:
    std::mutex mtx_;
    std::queue<job_t> jobs_;
  };
}
#endif //GTA_BASE_JOB_QUEUE_HPP
