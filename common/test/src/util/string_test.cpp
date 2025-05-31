//
// Created by X-ray on 05/04/2025.
//

#include <base-common/util/string.hpp>
#include <gtest/gtest.h>

TEST(String, ReplaceAll) {
  const std::string str = "Hello, world! Hello, world!";
  const std::string target_replace = "Hello";
  const std::string replace_val = "Hi";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hi, world! Hi, world!");
}