#
# The shared test/benchmark harness. Not part of the shipped library - nothing in src/ includes it,
# and asjit.hpp does not mention it - but kept as a target rather than a pair of source lists so that
# the test suite and the benchmark cannot end up building their engines differently.
#
cmake_minimum_required(VERSION 3.16)
project(asjit_support LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(${PROJECT_NAME} STATIC
        src/engine_harness.hpp
        src/engine_harness.cpp
)

add_library(base::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# AngelScript and AsmJit come through the alias, as they do for the test target.
target_link_libraries(${PROJECT_NAME} PUBLIC base::asjit)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
