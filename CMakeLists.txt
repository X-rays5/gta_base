cmake_minimum_required(VERSION 3.21)
project(gtabase)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/minhook)

add_library(gtabase SHARED src/dllmain.cpp src/main.cpp src/common/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/common/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/hooking/minhook/minhook.cpp src/hooking/minhook/minhook.hpp src/memory/sigscanner.hpp src/memory/pointers.cpp src/memory/pointers.hpp)

target_include_directories(gtabase PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)

target_link_libraries(gtabase PRIVATE minhook)
