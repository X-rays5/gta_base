//
// Created by X-ray on 02/10/23.
//

#pragma once
#ifndef GTA_BASE_SPINLOCK_82A7FE9438D94F2093F7FC89AF84E32E_HPP
#define GTA_BASE_SPINLOCK_82A7FE9438D94F2093F7FC89AF84E32E_HPP
#include <atomic>
#include <thread>

namespace gta_base::misc {
  class Spinlock {
  public:
    void Lock() noexcept {
      for (;;) {
        if (!lock_.exchange(true, std::memory_order_acquire)) {
          return;
        }

        while (lock_.load(std::memory_order_relaxed)) {
          std::this_thread::yield();
        }
      }
    }

    bool TryLock() noexcept {
      return !lock_.load(std::memory_order_relaxed) && !lock_.exchange(true, std::memory_order_acquire);
    }

    void Unlock() noexcept {
      lock_.store(false, std::memory_order_release);
    }

  private:
    std::atomic<bool> lock_ = false;
  };

  class RecursiveSpinlock {
  public:
    void Lock() noexcept {
      auto thread_id = std::this_thread::get_id();
      if (cur_locking_thread_.load(std::memory_order_consume) == thread_id) {
        lock_count_ += 1;
        return;
      }

      spinlock_.Lock();
      cur_locking_thread_.store(thread_id, std::memory_order_release);
      lock_count_ += 1;
    }

    bool TryLock() noexcept {
      auto thread_id = std::this_thread::get_id();

      if (cur_locking_thread_.load(std::memory_order_consume) == thread_id) {
        lock_count_ += 1;
        return true;
      }

      if (spinlock_.TryLock()) {
        cur_locking_thread_.store(thread_id, std::memory_order_release);
        lock_count_ += 1;
        return true;
      }

      return false;
    };

    void Unlock() noexcept {
      if (cur_locking_thread_.load(std::memory_order_acquire) != std::this_thread::get_id())
        return;

      lock_count_ -= 1;
      if (lock_count_ == 0) {
        cur_locking_thread_.store(default_thread_id_, std::memory_order_release);
        spinlock_.Unlock();
      }
    }

  private:
    Spinlock spinlock_;
    std::atomic<std::thread::id> cur_locking_thread_;
    std::thread::id default_thread_id_;
    std::size_t lock_count_{};
  };

  template<class T> requires std::is_same_v<T, Spinlock> or std::is_same_v<T, RecursiveSpinlock>
  class ScopedSpinlock {
  public:
    explicit ScopedSpinlock(T& lock) noexcept: lock_(lock) {
      lock_.Lock();
    }
    ~ScopedSpinlock() noexcept {
      lock_.Unlock();
    }

  private:
    T& lock_;
  };
}
#endif //GTA_BASE_SPINLOCK_82A7FE9438D94F2093F7FC89AF84E32E_HPP
