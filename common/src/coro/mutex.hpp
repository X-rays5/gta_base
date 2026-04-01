//
// Created by X-ray on 01/04/2026.
//

#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include "coroutine.hpp"

namespace base::common::coroutine {
  /**
   * @brief A true coroutine-aware mutex.
   *
   * Rather than spinning, when a coroutine attempts to lock this mutex and it is already locked,
   * it pushes itself to a wait queue and completely suspends itself (indefinitely) in the dispatcher.
   * It is only awakened uniquely when `unlock()` specifically hands the lock over to it, resulting
   * in zero processor spinning overhead.
   *
   * Meets the requirements of BasicLockable, so it can be used natively with std::lock_guard
   * and std::unique_lock.
   */
  class Mutex {
  public:
    Mutex() = default;
    ~Mutex() = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;
    Mutex& operator=(Mutex&&) = delete;

    /**
     * @brief Lock the mutex. Fully suspends the coroutine until ownership is explicitly transferred.
     */
    void lock() {
      void* handle = this_coro::get_current_handle();

      while (true) {
        {
          std::lock_guard lk(internal_mtx_);
          if (!locked_) {
            locked_ = true;
            return;
          }
          if (handle) {
            wait_queue_.push(handle);
          }
        }

        if (handle) {
          // Suspend indefinitely. We will only be woken up when `unlock()`
          // hands the lock over to us directly.
          this_coro::suspend_indefinitely();
          return;
        } else {
          // Fallback simple yield for non-coroutine OS Threads
          std::this_thread::yield();
        }
      }
    }

    /**
     * @brief Attempt to lock the mutex without yielding or suspending.
     * @return true if the lock was acquired, false otherwise.
     */
    bool try_lock() {
      std::lock_guard lk(internal_mtx_);
      if (!locked_) {
        locked_ = true;
        return true;
      }
      return false;
    }

    /**
     * @brief Unlock the mutex and transfer ownership to the next suspended coroutine.
     */
    void unlock() {
      void* next_handle = nullptr;

      {
        std::lock_guard lk(internal_mtx_);
        if (!wait_queue_.empty()) {
          next_handle = wait_queue_.front();
          wait_queue_.pop();
          // locked_ stays true, ownership transferred immediately
        } else {
          locked_ = false;
        }
      }

      if (next_handle) {
        // Wake the specific waiting coroutine directly
        this_coro::wake_handle(next_handle);
      }
    }

  private:
    bool locked_{false};
    std::queue<void*> wait_queue_;
    std::mutex internal_mtx_;
  };
}
