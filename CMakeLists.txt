cmake_minimum_required(VERSION 3.16)
project(gta_base)

message(STATUS "C Compiler path: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler path: ${CMAKE_CXX_COMPILER}")

if (NOT MSVC)
    MESSAGE(FATAL_ERROR "This project can only be built with MSVC.")
endif ()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_SOURCE_DIR}/.vendor)
include(cmake_scripts/get_cpm.cmake)

add_subdirectory(common)
add_subdirectory(imfont)
add_subdirectory(rage)
add_subdirectory(menu)
add_subdirectory(injector)

include(cmake_scripts/code_coverage.cmake)
if (OPENCPPCOVERAGE_EXECUTABLE)
    set(COVERAGE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/coverage")

    # Define the test targets
    set(ALL_TEST_TARGETS
            common_test
            imfont_test
            # Add other test targets as they're created
    )

    add_combined_coverage_target(
            coverage
            ${COVERAGE_OUTPUT_DIR}
            ${ALL_TEST_TARGETS}
    )
endif ()