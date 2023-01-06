//
// Created by X-ray on 08/12/22.
//

#pragma once
#ifndef GTA_BASE_POOL_HPP
#define GTA_BASE_POOL_HPP
#include <functional>
#include <mutex>
#include <queue>
#include <future>
#undef AddJob

namespace gta_base::fiber {
    class Pool {
    public:
      using job_t = std::function<void()>;

    public:
      explicit Pool(std::size_t num_per_tick);
      ~Pool();

      void AddJob(const job_t& job);
      template<typename T>
      [[nodiscard]] __forceinline std::future<T> AddJobFuture(std::function<T()> job) {
        auto task = std::make_shared<std::packaged_task<T()>>([job = std::move(job)]() -> T {
          return job();
        });
        std::future<T> future = task->get_future();

        AddJob([=]() {(*task)();});

        return future;
      }

      void Tick();
      [[noreturn]] static void FiberFunc();

    private:
      std::mutex mtx_;
      std::queue<job_t> jobs_;
    };
    inline Pool* kPOOL{};
  }
#endif //GTA_BASE_POOL_HPP
