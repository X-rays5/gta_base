//
// Created by X-ray on 05/04/2025.
//

#ifndef TIME_TEST_HPP_04234436
#define TIME_TEST_HPP_04234436
#include <chrono>
#include <base-common/util/time.hpp>
#include <gtest/gtest.h>

TEST(Time, TimePoint) {
    const auto now = base::common::util::time::GetTimeStamp();
    const auto also_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    ASSERT_TRUE(now <= also_now);
    EXPECT_GT(now + 5000, also_now);
}

#endif //TIME_TEST_HPP_04234436
