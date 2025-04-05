//
// Created by X-ray on 06/04/2025.
//

#ifndef VFS_TEST_HPP_04002216
#define VFS_TEST_HPP_04002216
#include <base-common/fs/vfs.hpp>
#include <gtest/gtest.h>

TEST(Vfs, SetWorkingDir) {
  // Get curr working dir
  const auto curr_dir = std::filesystem::current_path();

  base::common::fs::vfs::SetWorkingDir("unittest");

  // Check if the working dir is set correctly
  EXPECT_NE(std::filesystem::current_path().string().find("unittest"), std::string::npos);

  std::filesystem::current_path(curr_dir);
}

#endif //VFS_TEST_HPP_04002216
