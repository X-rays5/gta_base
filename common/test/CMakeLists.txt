cmake_minimum_required(VERSION 3.16)
project(common_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/win32/signal_test.cpp
        src/win32/signal_test.hpp
        src/win32/misc_test.cpp
        src/win32/misc_test.hpp
        src/test_globals.hpp
        src/util/spinlock_test.cpp
        src/util/spinlock_test.hpp
        src/util/string_test.cpp
        src/util/string_test.hpp
        src/util/time_trst.cpp
        src/util/time_trst.hpp
)

include(../../vendor/gtest.cmake)
enable_testing()

target_link_libraries(${PROJECT_NAME} PRIVATE base::common)

add_test(common ${PROJECT_NAME})