//
// Created by X-ray on 31/05/2025.
//
#include <base-common/util/ranged_value.hpp>
#include <gtest/gtest.h>

// Define a test fixture for RangedValue with a specific range (e.g., 0 to 100)
// This helps in setting up common objects for multiple tests.
template <typename T>
class RangedValueTest : public ::testing::Test {
protected:
    // Define a RangedValue type with a specific range for testing
    using TestRangedValue = base::common::util::RangedValue<T, static_cast<T>(0), static_cast<T>(100)>;

    // Helper function to get the min and max values for the current test type
    static constexpr T MinValue() {
        return static_cast<T>(0);
    }

    static constexpr T MaxValue() {
        return static_cast<T>(100);
    }
};

// Use a typed test suite to run tests for different arithmetic types (int, double)
using MyTypes = ::testing::Types<std::int32_t, std::double_t>;
TYPED_TEST_SUITE(RangedValueTest, MyTypes);

// Test case for the default constructor
TYPED_TEST(RangedValueTest, DefaultConstructorInitializesToMin) {
    typename TestFixture::TestRangedValue rv;
    ASSERT_EQ(rv, TestFixture::MinValue()); // Check if it equals the minimum value
}

// Test case for the value constructor with values within range
TYPED_TEST(RangedValueTest, ValueConstructorWithinRange) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(50));
    ASSERT_EQ(rv, static_cast<TypeParam>(50)); // Value should be unchanged
}

// Test case for the value constructor with values below min
TYPED_TEST(RangedValueTest, ValueConstructorClampsBelowMin) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(-10));
    ASSERT_EQ(rv, TestFixture::MinValue()); // Value should be clamped to min
}

// Test case for the value constructor with values above max
TYPED_TEST(RangedValueTest, ValueConstructorClampsAboveMax) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(150));
    ASSERT_EQ(rv, TestFixture::MaxValue()); // Value should be clamped to max
}

// Test case for implicit conversion to underlying type T
TYPED_TEST(RangedValueTest, ImplicitConversionToT) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(75));
    TypeParam val = rv; // Implicit conversion happens here
    ASSERT_EQ(val, static_cast<TypeParam>(75));

    // Test in a function call context
    auto check_val = [](TypeParam v) {
        return v == static_cast<TypeParam>(75);
    };
    ASSERT_TRUE(check_val(rv)); // rv implicitly converts to TypeParam
}

// Test case for an assignment operator with values within range
TYPED_TEST(RangedValueTest, AssignmentOperatorWithinRange) {
    typename TestFixture::TestRangedValue rv;
    rv = static_cast<TypeParam>(25);
    ASSERT_EQ(rv, static_cast<TypeParam>(25));
}

// Test case for an assignment operator with values below min
TYPED_TEST(RangedValueTest, AssignmentOperatorClampsBelowMin) {
    typename TestFixture::TestRangedValue rv;
    rv = static_cast<TypeParam>(-5);
    ASSERT_EQ(rv, TestFixture::MinValue());
}

// Test case for assignment operator with values above max
TYPED_TEST(RangedValueTest, AssignmentOperatorClampsAboveMax) {
    typename TestFixture::TestRangedValue rv;
    rv = static_cast<TypeParam>(101);
    ASSERT_EQ(rv, TestFixture::MaxValue());
}

// Test case for chained assignment
TYPED_TEST(RangedValueTest, ChainedAssignment) {
    typename TestFixture::TestRangedValue rv1, rv2;
    rv1 = rv2 = static_cast<TypeParam>(50); // rv2 gets 50, then rv1 gets rv2's value (50)
    ASSERT_EQ(rv1, static_cast<TypeParam>(50));
    ASSERT_EQ(rv2, static_cast<TypeParam>(50));

    rv1 = rv2 = static_cast<TypeParam>(200); // Both clamp to 100
    ASSERT_EQ(rv1, TestFixture::MaxValue());
    ASSERT_EQ(rv2, TestFixture::MaxValue());
}

// Test cases for operator== with raw T
TYPED_TEST(RangedValueTest, EqualityOperatorWithT) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(50));
    ASSERT_TRUE(rv == static_cast<TypeParam>(50));
    ASSERT_FALSE(rv == static_cast<TypeParam>(49));
    ASSERT_FALSE(rv == static_cast<TypeParam>(51));
}

// Test cases for operator<=> with raw T
TYPED_TEST(RangedValueTest, SpaceshipOperatorWithT) {
    typename TestFixture::TestRangedValue rv(static_cast<TypeParam>(50));
    ASSERT_TRUE((rv <=> static_cast<TypeParam>(40)) == std::strong_ordering::greater);
    ASSERT_TRUE((rv <=> static_cast<TypeParam>(50)) == std::strong_ordering::equal);
    ASSERT_TRUE((rv <=> static_cast<TypeParam>(60)) == std::strong_ordering::less);
}

// Test cases for operator== with RangedValue
TYPED_TEST(RangedValueTest, EqualityOperatorWithRangedValue) {
    typename TestFixture::TestRangedValue rv1(static_cast<TypeParam>(50));
    typename TestFixture::TestRangedValue rv2(static_cast<TypeParam>(50));
    typename TestFixture::TestRangedValue rv3(static_cast<TypeParam>(60));

    ASSERT_TRUE(rv1 == rv2);
    ASSERT_FALSE(rv1 == rv3);
}

// Test cases for operator<=> with RangedValue
TYPED_TEST(RangedValueTest, SpaceshipOperatorWithRangedValue) {
    typename TestFixture::TestRangedValue rv1(static_cast<TypeParam>(50));
    typename TestFixture::TestRangedValue rv2(static_cast<TypeParam>(40));
    typename TestFixture::TestRangedValue rv3(static_cast<TypeParam>(50));
    typename TestFixture::TestRangedValue rv4(static_cast<TypeParam>(60));

    ASSERT_TRUE((rv1 <=> rv2) == std::strong_ordering::greater);
    ASSERT_TRUE((rv1 <=> rv3) == std::strong_ordering::equal);
    ASSERT_TRUE((rv1 <=> rv4) == std::strong_ordering::less);
}

// Test with a different range (e.g., -10 to 10)
TEST(RangedValueSpecificRangeTest, IntRangeNegativeToPositive) {
    using RangedInt = base::common::util::RangedValue<std::int32_t, -10, 10>;

    const RangedInt rv_default;
    ASSERT_EQ(rv_default, -10); // Default should be min

    RangedInt rv1(5);
    ASSERT_EQ(rv1, 5);

    const RangedInt rv2(-15); // Clamped to -10
    ASSERT_EQ(rv2, -10);

    const RangedInt rv3(20); // Clamped to 10
    ASSERT_EQ(rv3, 10);

    rv1 = -20; // Clamped to -10
    ASSERT_EQ(rv1, -10);

    rv1 = 15; // Clamped to 10
    ASSERT_EQ(rv1, 10);
}

// Test with default numeric_limits range (full range)
TEST(RangedValueDefaultRangeTest, FullRangeInt) {
    using RangedIntDefault = base::common::util::RangedValue<std::int32_t>; // Uses std::numeric_limits<std::int32_t>::min/max

    const RangedIntDefault rv_min(std::numeric_limits<std::int32_t>::min());
    ASSERT_EQ(rv_min, std::numeric_limits<std::int32_t>::min());

    const RangedIntDefault rv_max(std::numeric_limits<std::int32_t>::max());
    ASSERT_EQ(rv_max, std::numeric_limits<std::int32_t>::max());

    // Test values slightly outside min/max (should still be min/max due to clamping)
    // Note: This is mostly for completeness; for default limits,
    // it's hard to get a value *outside* the limits without overflow.
    // For int, min-1 would overflow to max, max+1 would overflow to min.
    // So these tests primarily confirm behavior at the boundaries.
    const RangedIntDefault rv_assign_min = std::numeric_limits<std::int32_t>::min();
    ASSERT_EQ(rv_assign_min, std::numeric_limits<std::int32_t>::min());

    const RangedIntDefault rv_assign_max = std::numeric_limits<std::int32_t>::max();
    ASSERT_EQ(rv_assign_max, std::numeric_limits<std::int32_t>::max());
}

// Test with default numeric_limits range for double
TEST(RangedValueDefaultRangeTest, FullRangeDouble) {
    using RangedDoubleDefault = base::common::util::RangedValue<std::double_t>;

    const RangedDoubleDefault rv_min(std::numeric_limits<std::double_t>::lowest()); // Use lowest for floating points
    ASSERT_EQ(rv_min, std::numeric_limits<std::double_t>::lowest());

    const RangedDoubleDefault rv_max(std::numeric_limits<std::double_t>::max());
    ASSERT_EQ(rv_max, std::numeric_limits<std::double_t>::max());

    // Test values just outside the effective range (e.g., slightly larger than max)
    // This is more meaningful for floating-point types where direct overflow isn't the primary concern.
    const RangedDoubleDefault rv_assign_slightly_above_max = std::numeric_limits<std::double_t>::max() * 1.1; // A value clearly above max
    ASSERT_EQ(rv_assign_slightly_above_max, std::numeric_limits<std::double_t>::max());

    const RangedDoubleDefault rv_assign_slightly_below_min = std::numeric_limits<std::double_t>::lowest() * 1.1; // A value clearly below lowest
    ASSERT_EQ(rv_assign_slightly_below_min, std::numeric_limits<std::double_t>::lowest());
}
