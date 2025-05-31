//
// Created by X-ray on 05/04/2025.
//

#include <future>
#include <base-common/concurrency/spinlock.hpp>
#include <gtest/gtest.h>

inline std::atomic<bool> keep_lock = true;

template <class T> requires std::is_same_v<T, base::common::concurrency::Spinlock> or std::is_same_v<T, base::common::concurrency::RecursiveSpinlock>
void WaitForUnlock(T& spinlock) {
    base::common::concurrency::ScopedSpinlock<T> lock(spinlock);

    const auto start_time = std::chrono::steady_clock::now();
    constexpr auto timeout = std::chrono::seconds(5); // After 5 seconds we can safely say this isn't going to work

    while (keep_lock) {
        std::this_thread::yield();

        // Check if we've exceeded the timeout
        if (std::chrono::steady_clock::now() - start_time > timeout) {
            throw std::runtime_error("WaitForUnlock: Has timed out");
        }
    }
}

TEST(SpinLock, TryLock) {
    base::common::concurrency::Spinlock spinlock;
    keep_lock = true;

    const auto fut = std::async(std::launch::async, WaitForUnlock<base::common::concurrency::Spinlock>, std::ref(spinlock));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Need to make sure the new thread actually had time to take the lock

    ASSERT_FALSE(spinlock.TryLock());
    keep_lock = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_TRUE(spinlock.TryLock());
}

TEST(SpinLock, LockUnlock) {
    base::common::concurrency::Spinlock spinlock;
    keep_lock = false;

    spinlock.Lock();

    const auto fut = std::async(std::launch::async, WaitForUnlock<base::common::concurrency::Spinlock>, std::ref(spinlock));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_NE(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);

    spinlock.Unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_EQ(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);
}

// Now for recursive spinlock
TEST(RecursiveSpinLock, TryLock) {
    base::common::concurrency::RecursiveSpinlock spinlock;
    keep_lock = true;

    auto fut = std::async(std::launch::async, WaitForUnlock<base::common::concurrency::RecursiveSpinlock>, std::ref(spinlock));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Need to make sure the new thread actually had time to take the lock

    ASSERT_FALSE(spinlock.TryLock());
    keep_lock = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_EQ(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);

    ASSERT_TRUE(spinlock.TryLock());
    ASSERT_TRUE(spinlock.TryLock());

    fut = std::async(std::launch::async, WaitForUnlock<base::common::concurrency::RecursiveSpinlock>, std::ref(spinlock));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_NE(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);

    keep_lock = false;
    spinlock.Unlock();
    spinlock.Unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_EQ(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);
}

TEST(RecursiveSpinLock, LockUnlock) {
    base::common::concurrency::RecursiveSpinlock spinlock;
    keep_lock = false;

    spinlock.Lock();

    const auto fut = std::async(std::launch::async, WaitForUnlock<base::common::concurrency::RecursiveSpinlock>, std::ref(spinlock));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_NE(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);

    spinlock.Unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_EQ(fut.wait_for(std::chrono::seconds(0)), std::future_status::ready);
}
