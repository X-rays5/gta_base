cmake_minimum_required(VERSION 3.16)
project(gta_base)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_SOURCE_DIR}/.vendor)
include(cmake_scripts/get_cpm.cmake)

add_subdirectory(common)
add_subdirectory(imfont)

if (WIN32)
    if (MSVC)
        add_subdirectory(menu)
    endif ()
endif ()

add_subdirectory(injector)

if (WIN32)
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
endif ()