cmake_minimum_required(VERSION 3.16)
project(gta_base)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake_scripts/get_cpm.cmake)

add_subdirectory(common)
add_subdirectory(imfont)

if (WIN32)
    if (MSVC)
        add_subdirectory(menu)
    endif ()
endif ()

add_subdirectory(injector)

# Add to the end of your root CMakeLists.txt
if (WIN32)
    include(cmake_scripts/code_coverage.cmake)
    if (OPENCPPCOVERAGE_EXECUTABLE)
        add_custom_target(coverage
                COMMENT "Generate coverage for all tests"
                DEPENDS coverage_common_test
                # Add other test targets as they're created
        )
    endif ()
endif ()