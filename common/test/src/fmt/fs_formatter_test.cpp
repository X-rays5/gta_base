//
// Created by X-ray on 06/04/2025.
//

#include <base-common/fmt/fs_formatter.hpp>
#include <gtest/gtest.h>

TEST(FsFormatter, FormatPath) {
  const std::string str_path = "C:/Users/X-ray/Documents/test.txt";
  const std::filesystem::path path = str_path;

  EXPECT_EQ(fmt::format("{}", path), str_path);
  EXPECT_EQ(fmt::format("{}", path), path.generic_string());
}
