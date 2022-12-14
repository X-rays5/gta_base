cmake_minimum_required(VERSION 3.21)
project(crossmap_gen)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp src/get_files.cpp src/get_files.hpp src/script_file.cpp src/script_file.hpp src/opcodes.hpp)

include(../vendor/fmt.cmake)
include(../vendor/glog.cmake)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${fmt_SOURCE_DIR}/include/fmt/format.h ${glog_BINARY_DIR}/glog/logging.h)