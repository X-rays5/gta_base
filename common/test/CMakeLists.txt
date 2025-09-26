cmake_minimum_required(VERSION 3.26)
project(common_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/win32/signal_test.cpp
        src/win32/misc_test.cpp
        src/test_globals.hpp
        src/concurrency/spinlock_test.cpp
        src/util/string_test.cpp
        src/util/time_test.cpp
        src/conversion/hex_test.cpp
        src/fmt/fs_formatter_test.cpp
        src/fs/vfs_test.cpp
        src/memory/address_test.cpp
        src/util/ranged_value.cpp
)

include(../../vendor/gtest.cmake)
enable_testing()

target_link_libraries(${PROJECT_NAME} PRIVATE base::common)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})