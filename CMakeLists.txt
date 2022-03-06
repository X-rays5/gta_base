cmake_minimum_required(VERSION 3.21)
project(gtabase)

set(CMAKE_CXX_STANDARD 20)

add_library(gtabase SHARED src/dllmain.cpp src/main.cpp src/common/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/common/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp)

target_include_directories(gtabase PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)
