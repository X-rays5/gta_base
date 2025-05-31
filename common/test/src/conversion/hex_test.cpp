//
// Created by X-ray on 06/04/2025.
//

#include <base-common/conversion/hex.hpp>
#include <gtest/gtest.h>

TEST(Hex, AddrToHex) {
  EXPECT_EQ(base::common::conversion::AddrToHex(0x0), "0x0");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x1), "0x1");
  EXPECT_EQ(base::common::conversion::AddrToHex(0xA), "0xA");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x10), "0x10");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x100), "0x100");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x1000), "0x1000");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x10000), "0x10000");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x100000), "0x100000");
  EXPECT_EQ(base::common::conversion::AddrToHex(0x1000000), "0x1000000");
  EXPECT_EQ(base::common::conversion::AddrToHex(0xFFFFFFFF), "0xFFFFFFFF");
  EXPECT_EQ(base::common::conversion::AddrToHex(0xFFFFFFFFFFFFFFFF), "0xFFFFFFFFFFFFFFFF");
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
  EXPECT_EQ(base::common::conversion::HexCharToInt('a').value(), 10);
  EXPECT_EQ(base::common::conversion::HexCharToInt('b').value(), 11);
  EXPECT_EQ(base::common::conversion::HexCharToInt('c').value(), 12);
  EXPECT_EQ(base::common::conversion::HexCharToInt('d').value(), 13);
  EXPECT_EQ(base::common::conversion::HexCharToInt('e').value(), 14);
  EXPECT_EQ(base::common::conversion::HexCharToInt('f').value(), 15);

  auto res = base::common::conversion::HexCharToInt('G');
  ASSERT_TRUE(res.error());
  auto res2 = base::common::conversion::HexCharToInt('g');
  ASSERT_TRUE(res2.error());
  auto res3 = base::common::conversion::HexCharToInt(' ');
  ASSERT_TRUE(res3.error());
  auto res4 = base::common::conversion::HexCharToInt('!');
  ASSERT_TRUE(res4.error());
}