//
// Created by X-ray on 06/04/2025.
//

#include <base-common/memory/address.hpp>
#include <gtest/gtest.h>
#include <limits>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

TEST(Address, Constructor) {
  // Test with a null pointer
  const base::common::memory::Address null_addr(nullptr);
  EXPECT_EQ(static_cast<void*>(null_addr), nullptr);

  // Test with a specific address
  constexpr std::uintptr_t specific_address = 0x12345678;
  const base::common::memory::Address addr(specific_address);
  EXPECT_EQ(static_cast<std::uintptr_t>(addr), specific_address);
}

TEST(Address, As) {
  std::int32_t test = 3;

  base::common::memory::Address addr(&test);
  EXPECT_EQ(addr.As<int*>(), &test);

  //Test with uintptr_t
  const auto address_value = reinterpret_cast<std::uintptr_t>(&test);
  base::common::memory::Address uintptr_addr(address_value);
  EXPECT_EQ(uintptr_addr.As<std::uintptr_t>(), address_value);
}

TEST(Address, Sub) {
  std::int32_t test = 3;

  const base::common::memory::Address addr(&test);
  EXPECT_EQ(addr.Sub(4).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) - 4));

  // Test with a larger offset
  std::uintptr_t offset = 0x100;
  EXPECT_EQ(addr.Sub(offset).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) - offset));

  // Test with maximum offset (std::numeric_limits<std::uintptr_t>::max()) - might cause issues, be careful.
  offset = std::numeric_limits<std::uintptr_t>::max(); // commented out because it will cause an overflow
  EXPECT_EQ(addr.Sub(offset).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) - offset));
}

TEST(Address, Add) {
  std::int32_t test = 3;

  const base::common::memory::Address addr(&test);
  EXPECT_EQ(addr.Add(4).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + 4));

  // Test with a larger offset
  std::uintptr_t offset = 0x100;
  EXPECT_EQ(addr.Add(offset).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + offset));

  // Test with maximum offset (std::numeric_limits<std::uintptr_t>::max()) - might cause issues, be careful
  offset = std::numeric_limits<std::uintptr_t>::max(); // commented out because it will cause an overflow
  EXPECT_EQ(addr.Add(offset).As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + offset));
}

TEST(Address, Rip) {
  std::int32_t test = 3;

  base::common::memory::Address addr(&test);
  EXPECT_EQ(addr.Rip().As<int*>(), reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(&test) + test + 4));
}

TEST(Address, GetMemoryInformation) {
  std::int32_t test = 3;

  base::common::memory::Address addr(&test);
  auto memory_info = addr.GetMemoryInformation();

  ASSERT_FALSE(memory_info.error());

  // Check that addr is within the memory region
  const auto base_addr = reinterpret_cast<std::uintptr_t>(memory_info.value().BaseAddress);
  const std::uintptr_t region_size = memory_info.value().RegionSize;
  const auto addr_value = addr.As<std::uintptr_t>();

  EXPECT_GE(addr_value, base_addr);
  EXPECT_LT(addr_value, base_addr + region_size);

  // Test with a nullptr - should not return an error
  base::common::memory::Address null_addr(nullptr);
  const auto null_memory_info = null_addr.GetMemoryInformation();
  ASSERT_FALSE(null_memory_info.error());
}

TEST(Address, OperatorVoidPtr) {
  std::int32_t test = 3;
  const base::common::memory::Address addr(&test);
  const auto* void_ptr = static_cast<void*>(addr);
  EXPECT_EQ(void_ptr, &test);

  // Test with nullptr
  const base::common::memory::Address null_addr(nullptr);
  const auto* null_void_ptr = static_cast<void*>(null_addr);
  EXPECT_EQ(null_void_ptr, nullptr);
}

TEST(Address, OperatorUintptrT) {
  std::int32_t test = 3;
  const base::common::memory::Address addr(&test);
  const auto uintptr_value = static_cast<std::uintptr_t>(addr);
  EXPECT_EQ(uintptr_value, reinterpret_cast<std::uintptr_t>(&test));

  // Test with a specific address
  constexpr std::uintptr_t specific_address = 0xABCDEF0123456789U;
  const base::common::memory::Address specific_addr(specific_address);
  const auto specific_uintptr_value = static_cast<std::uintptr_t>(specific_addr);
  EXPECT_EQ(specific_uintptr_value, specific_address);
}