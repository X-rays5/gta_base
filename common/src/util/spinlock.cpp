//
// Created by X-ray on 05/11/2023.
//

#include "spinlock.hpp"

namespace base::common::util {
  void Spinlock::Lock() noexcept {
    for (;;) {
      if (!lock_.exchange(true, std::memory_order_acquire)) {
        return;
      }

      while (lock_.load(std::memory_order_relaxed)) {
        std::this_thread::yield();
      }
    }
  }

  bool Spinlock::TryLock() noexcept {
    return !lock_.load(std::memory_order_relaxed) && !lock_.exchange(true, std::memory_order_acquire);
  }

  void Spinlock::Unlock() noexcept {
    lock_.store(false, std::memory_order_release);
  }

  void RecursiveSpinlock::Lock() noexcept {
    const auto thread_id = std::this_thread::get_id();
    if (cur_locking_thread_.load(std::memory_order_consume) == thread_id) {
      lock_count_ += 1;
      return;
    }

    spinlock_.Lock();
    cur_locking_thread_.store(thread_id, std::memory_order_release);
    lock_count_ += 1;
  }

  bool RecursiveSpinlock::TryLock() noexcept {
    const auto thread_id = std::this_thread::get_id();

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
  }

  void RecursiveSpinlock::Unlock() noexcept {
    if (cur_locking_thread_.load(std::memory_order_acquire) != std::this_thread::get_id())
      return;

    lock_count_ -= 1;
    if (lock_count_ == 0) {
      cur_locking_thread_.store(default_thread_id_, std::memory_order_release);
      spinlock_.Unlock();
    }
  }
}
