#include <base-common/util/string.hpp>
#include <gtest/gtest.h>

TEST(String, ReplaceAll) {
  const std::string str = "Hello, world! Hello, world!";
  const std::string target_replace = "Hello";
  const std::string replace_val = "Hi";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hi, world! Hi, world!");
}

TEST(String, ReplaceAll_EmptyString) {
  const std::string str = "";
  const std::string target_replace = "Hello";
  const std::string replace_val = "Hi";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "");
}

TEST(String, ReplaceAll_EmptyTarget) {
  const std::string str = "Hello, world!";
  const std::string target_replace = "";
  const std::string replace_val = "Hi";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hello, world!");
}

TEST(String, ReplaceAll_TargetNotFound) {
  const std::string str = "Hello, world!";
  const std::string target_replace = "Goodbye";
  const std::string replace_val = "Hi";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hello, world!");
}

TEST(String, ReplaceAll_OverlappingTargets) {
  const std::string str = "abababa";
  const std::string target_replace = "aba";
  const std::string replace_val = "c";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "cbc");
}

TEST(String, ReplaceAll_DifferentLengths) {
  const std::string str = "Hello";
  const std::string target_replace = "Hello";
  const std::string replace_val = "Hi there";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hi there");
}

TEST(String, ReplaceAll_SpecialCharacters) {
  const std::string str = "Hello, world!@#$%^&*()_+=-`~[]\\{}|;':\",./<>?";
  const std::string target_replace = "@#$%^&*()_+=-`~[]\\{}|;':\",./<>?";
  const std::string replace_val = "!";

  const std::string result = base::common::util::string::ReplaceAll(str, target_replace, replace_val);

  ASSERT_EQ(result, "Hello, world!!");
}