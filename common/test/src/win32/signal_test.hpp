//
// Created by X-ray on 05/04/2025.
//

#ifndef SIGNAL_TEST_HPP_04163912
#define SIGNAL_TEST_HPP_04163912
#include <thread>
#include <base-common/util/time.hpp>
#include <base-common/win32/signal.hpp>
#include <gtest/gtest.h>

TEST(Win32Signal, StopWaitAfterTimeout) {
    const base::win32::Signal signal;

    const auto time_now = base::common::util::time::GetTimeStamp();

    signal.Wait(2000);

    const auto time_after = base::common::util::time::GetTimeStamp();
    const auto time_diff = time_after - time_now;

    EXPECT_GE(time_diff, 1900);
    EXPECT_LE(time_diff, 3000);
}

inline void WaitTillSignalRelease(const base::win32::Signal& signal, const std::int64_t time_till_release) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time_till_release));
    signal.Notify();
}

TEST(Win32Signal, WaitForNotify) {
    constexpr std::int64_t release_after = 2000;
    const base::win32::Signal signal;

    std::jthread thread(WaitTillSignalRelease, std::ref(signal), release_after);

    const auto time_now = base::common::util::time::GetTimeStamp();

    signal.Wait();

    const auto time_after = base::common::util::time::GetTimeStamp();
    const auto time_diff = time_after - time_now;

    EXPECT_GE(time_diff, release_after);
    EXPECT_LE(time_diff, release_after + 1000);
}

#endif //SIGNAL_TEST_HPP_04163912
