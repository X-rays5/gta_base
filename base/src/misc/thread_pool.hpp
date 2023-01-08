//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_THREAD_POOL_HPP
#define GTA_BASE_THREAD_POOL_HPP
#include <condition_variable>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include <thread>
#include <vector>
#include <future>
#include <atomic>

namespace gta_base::misc {
  class ThreadPool {
  public:
    ThreadPool();
    explicit ThreadPool(std::size_t thread_count);
    ~ThreadPool();

    void AddJob(std::function<void()> task);

    template<typename T>
    [[nodiscard]] FORCE_INLINE std::future<T> AddJobFuture(std::function<T()> task) {
      auto task_wrapper = std::make_shared<std::packaged_task<T()>>([task = std::move(task)]() -> T {
        return task();
      });
      std::future<T> future = task_wrapper->get_future();

      AddJob([=]() { (*task_wrapper)(); });

      return future;
    }

    std::size_t GetThreadCount() const;
  private:
    std::mutex mtx_;
    std::atomic<bool> running_ = true;
    std::queue<std::function<void()>> tasks_;
    std::vector<std::thread> threads_;
    std::condition_variable job_notify_;

  private:
    static void WorkerLoop(ThreadPool* pool);
    void CreateThreads(std::size_t thread_count);
  };

  inline ThreadPool* kTHREAD_POOL{};
}
#endif //GTA_BASE_THREAD_POOL_HPP
