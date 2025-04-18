//
// Created by X-ray on 06/04/2025.
//

#ifndef HEX_TEST_HPP_04000549
#define HEX_TEST_HPP_04000549
#include <base-common/conversion/hex.hpp>
#include <gtest/gtest.h>

TEST(Hex, AddrToHex) {
    EXPECT_EQ(base::common::conversion::AddrToHex(0x0), "0x0");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x1), "0x1");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x10), "0x10");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x100), "0x100");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x1000), "0x1000");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x10000), "0x10000");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x100000), "0x100000");
    EXPECT_EQ(base::common::conversion::AddrToHex(0x1000000), "0x1000000");
}

TEST(Hex, HexCharToInt) {
    EXPECT_EQ(base::common::conversion::HexCharToInt('0').value(), 0);
    EXPECT_EQ(base::common::conversion::HexCharToInt('1').value(), 1);
    EXPECT_EQ(base::common::conversion::HexCharToInt('2').value(), 2);
    EXPECT_EQ(base::common::conversion::HexCharToInt('3').value(), 3);
    EXPECT_EQ(base::common::conversion::HexCharToInt('4').value(), 4);
    EXPECT_EQ(base::common::conversion::HexCharToInt('5').value(), 5);
    EXPECT_EQ(base::common::conversion::HexCharToInt('6').value(), 6);
    EXPECT_EQ(base::common::conversion::HexCharToInt('7').value(), 7);
    EXPECT_EQ(base::common::conversion::HexCharToInt('8').value(), 8);
    EXPECT_EQ(base::common::conversion::HexCharToInt('9').value(), 9);
    EXPECT_EQ(base::common::conversion::HexCharToInt('A').value(), 10);
    EXPECT_EQ(base::common::conversion::HexCharToInt('B').value(), 11);
    EXPECT_EQ(base::common::conversion::HexCharToInt('C').value(), 12);
    EXPECT_EQ(base::common::conversion::HexCharToInt('D').value(), 13);
    EXPECT_EQ(base::common::conversion::HexCharToInt('E').value(), 14);
    EXPECT_EQ(base::common::conversion::HexCharToInt('F').value(), 15);

    auto res = base::common::conversion::HexCharToInt('G');
    ASSERT_TRUE(res.error());
}

#endif //HEX_TEST_HPP_04000549
