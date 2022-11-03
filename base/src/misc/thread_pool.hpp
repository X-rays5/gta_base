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
    using job_t = std::function<void()>;
    using cb_t = std::function<void()>;

    struct Job {
      job_t job;
      cb_t cb;
    };

  public:
    ThreadPool();
    explicit ThreadPool(std::size_t thread_count);
    ~ThreadPool();

    void AddJob(job_t task, cb_t callback = nullptr);

    std::size_t GetThreadCount() const;
  private:
    std::mutex mtx_;
    std::atomic<bool> running_ = true;
    std::queue<Job> tasks_;
    std::vector<std::thread> threads_;
    std::condition_variable job_notify_;

  private:
    static void WorkerLoop(ThreadPool* pool);
    void CreateThreads(std::size_t thread_count);

  };
  inline ThreadPool* kTHREAD_POOL{};
}
#endif //GTA_BASE_THREAD_POOL_HPP
