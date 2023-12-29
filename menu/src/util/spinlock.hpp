//
// Created by X-ray on 02/10/23.
//

#pragma once
#ifndef GTA_BASE_SPINLOCK_82A7FE9438D94F2093F7FC89AF84E32E_HPP
#define GTA_BASE_SPINLOCK_82A7FE9438D94F2093F7FC89AF84E32E_HPP
#include <atomic>
#include <thread>

namespace base::util {
  class Spinlock {
  public:
    Spinlock() = default;
    ~Spinlock() = default;

    Spinlock(const Spinlock& c) = delete;
    void operator=(const Spinlock& c) = delete;
    Spinlock(Spinlock&& c) = delete;
    void operator=(Spinlock&& c) = delete;

    void Lock() noexcept;
    bool TryLock() noexcept;
    void Unlock() noexcept;

  private:
    std::atomic<bool> lock_ = false;
  };

  class RecursiveSpinlock {
  public:
    RecursiveSpinlock() = default;
    ~RecursiveSpinlock() = default;

    RecursiveSpinlock(const RecursiveSpinlock& c) = delete;
    void operator=(const RecursiveSpinlock& c) = delete;
    RecursiveSpinlock(RecursiveSpinlock&& c) = delete;
    void operator=(RecursiveSpinlock&& c) = delete;

    void Lock() noexcept;
    bool TryLock() noexcept;
    void Unlock() noexcept;

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