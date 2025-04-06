//
// Created by X-ray on 06/04/2025.
//

#ifndef ADDRESS_TEST_HPP_04002840
#define ADDRESS_TEST_HPP_04002840
#include <base-common/memory/address.hpp>
#include <gtest/gtest.h>

TEST(Address, As) {
    int test = 3;

    base::common::memory::Address addr(&test);
    EXPECT_EQ(addr.As<int*>(), &test);
}

TEST(Address, Sub) {
    int test = 3;

    const base::common::memory::Address addr(&test);
    EXPECT_EQ(addr.Sub(4).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) - 4));
}

TEST(Address, Add) {
    int test = 3;

    const base::common::memory::Address addr(&test);
    EXPECT_EQ(addr.Add(4).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + 4));
}

TEST(Address, Rip) {
    int test = 3;

    base::common::memory::Address addr(&test);
    EXPECT_EQ(addr.Rip().As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + test + 4));
}

TEST(Address, GetMemoryInformation) {
    int test = 3;

    base::common::memory::Address addr(&test);
    auto memory_info = addr.GetMemoryInformation();

    ASSERT_FALSE(memory_info.error());

    // Check that addr is within the memory region
    const std::uintptr_t base_addr = reinterpret_cast<std::uintptr_t>(memory_info.value().BaseAddress);
    const std::uintptr_t region_size = static_cast<std::uintptr_t>(memory_info.value().RegionSize);
    const std::uintptr_t addr_value = addr.As<std::uintptr_t>();

    EXPECT_GE(addr_value, base_addr);
    EXPECT_LT(addr_value, base_addr + region_size);
}

#endif //ADDRESS_TEST_HPP_04002840
