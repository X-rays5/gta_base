cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR
            "VCPKG_ROOT environment variable is not set.\n"
            "Please set VCPKG_ROOT to your vcpkg installation directory.\n"
            "See: https://learn.microsoft.com/en-us/vcpkg/users/config-environment"
    )
endif()

set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE FILEPATH "Vcpkg toolchain file"
)

set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "Vcpkg target triplet")
set(VCPKG_HOST_TRIPLET "x64-windows-static-md" CACHE STRING "Vcpkg host triplet")

# Keep vcpkg state inside the CMake build directory.
set(VCPKG_INSTALLED_DIR
        "${CMAKE_BINARY_DIR}/vcpkg_installed"
        CACHE PATH "vcpkg installed directory"
)

set(VCPKG_DOWNLOADS
        "${CMAKE_BINARY_DIR}/vcpkg_downloads"
        CACHE PATH "vcpkg downloads directory"
)

set(VCPKG_BINARY_SOURCES
        "clear;files,${CMAKE_BINARY_DIR}/vcpkg_cache"
        CACHE STRING "vcpkg binary cache"
)

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
add_subdirectory(asjit)
add_subdirectory(menu)
add_subdirectory(injector)

# Standalone developer tools. Off by default would be surprising; opt out to skip the
# curl download cpr needs (it does not go through CPM_SOURCE_CACHE).
option(BASE_BUILD_TOOLS "Build standalone developer tools" ON)
if (BASE_BUILD_TOOLS)
    add_subdirectory(tools)
endif ()

include(cmake_scripts/code_coverage.cmake)
if (OPENCPPCOVERAGE_EXECUTABLE)
    set(COVERAGE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/coverage")

    # Define the test targets
    set(ALL_TEST_TARGETS
            common_test
            imfont_test
            asjit_test
            menu_test
            injector_test
    )

    add_combined_coverage_target(
            coverage
            ${COVERAGE_OUTPUT_DIR}
            ${ALL_TEST_TARGETS}
    )
endif ()