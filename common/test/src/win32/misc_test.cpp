//
// Created by X-ray on 05/04/2025.
//

#include <Windows.h>
#include <base-common/win32/misc.hpp>
#include <gtest/gtest.h>
#include "../test_globals.hpp"

TEST(Win32Misc, GetKnownFolderPath) {
  using namespace base::win32;
  auto result = GetKnownFolderPath(KNOWN_FOLDER_ID::kDocuments);
  ASSERT_TRUE(result.has_value()) << "Failed to get known folder path. Error: " << result.error();
  EXPECT_FALSE(result.value().empty()) << "The path should not be empty.";
}

TEST(Win32Misc, GetProcessModules) {
  using namespace base::win32;
  auto result = GetProcessModules(GetCurrentProcessId());
  ASSERT_TRUE(result.has_value()) << "Failed to get process modules. Error: " << result.error();
  EXPECT_FALSE(result.value().empty()) << "The module list should not be empty.";
}

TEST(Win32Misc, GetHwnd) {
  using namespace base::win32;
  CreateDummyWindow(test_dummy_window_name, test_dummy_window_class);

  auto result = GetHwnd(test_dummy_window_class, test_dummy_window_name);
  ASSERT_TRUE(result.has_value()) << "Failed to get hwnd. Error: " << result.error();
  HWND hwnd = result.value();
  ASSERT_NE(hwnd, nullptr) << "The hwnd should not be null.";

  DestroyDummyWindow(hwnd);
}

TEST(Win32Misc, GetGameHwnd) {
  using namespace base::win32;
  const auto result = GetGameHwnd();
  ASSERT_TRUE(!result.has_value());
  ASSERT_TRUE(result.error());
}

TEST(Win32Misc, GetPIDFromHWND) {
  using namespace base::win32;
  CreateDummyWindow(test_dummy_window_name, test_dummy_window_class);

  auto hwnd_result = GetHwnd(test_dummy_window_class, test_dummy_window_name);
  ASSERT_TRUE(hwnd_result.has_value()) << "Failed to get hwnd. Error: " << hwnd_result.error();
  auto pid_result = GetPIDFromHWND(hwnd_result.value());
  ASSERT_TRUE(pid_result.has_value()) << "Failed to get pid from hwnd. Error: " << pid_result.error();
  EXPECT_EQ(pid_result.value(), GetCurrentProcessId()) << "The pid should match the current process id.";

  DestroyDummyWindow(hwnd_result.value());
}

TEST(Win32Misc, GetLastErrorStr) {
  using namespace base::win32;

  // cause an error by passing an invalid handle to a win32 api function
  OpenProcess(PROCESS_ALL_ACCESS, FALSE, 0xFFFFFFFF);

  auto result = GetLastErrorStr();
  ASSERT_FALSE(result.empty()) << "The error string should not be empty.";
  EXPECT_NE(result.find("Error"), std::string::npos) << "The error string should contain 'Error'.";
}
