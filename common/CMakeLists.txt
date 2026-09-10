cmake_minimum_required(VERSION 3.16)
project(common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(${PROJECT_NAME} STATIC
        src/base-common/globals.hpp
        src/base-common/concurrency/spinlock.cpp
        src/base-common/concurrency/spinlock.hpp
        src/base-common/util/result.cpp
        src/base-common/util/result.hpp
        src/base-common/fmt/fs_formatter.cpp
        src/base-common/fmt/fs_formatter.hpp
        src/base-common/util/string.cpp
        src/base-common/util/string.hpp
        src/base-common/util/time.cpp
        src/base-common/util/time.hpp
        src/base-common/conversion/hex.cpp
        src/base-common/conversion/hex.hpp
        src/base-common/logging/logger.hpp
        src/base-common/logging/logger.cpp
        src/base-common/logging/logging_macro.hpp
        src/base-common/logging/exception/exception_report.cpp
        src/base-common/logging/exception/exception_report.hpp
        src/base-common/logging/exception/util.hpp
        src/base-common/logging/exception/util.cpp
        src/base-common/logging/exception/vectored_handler.hpp
        src/base-common/logging/exception/vectored_handler.cpp
        src/base-common/logging/formatter/thread_id.hpp
        src/base-common/win32/all.hpp
        src/base-common/win32/memory.hpp
        src/base-common/win32/memory.cpp
        src/base-common/win32/misc.hpp
        src/base-common/win32/misc.cpp
        src/base-common/win32/signal.hpp
        src/base-common/fs/vfs.cpp
        src/base-common/fs/vfs.hpp
        src/base-common/memory/address.cpp
        src/base-common/memory/address.hpp
        src/base-common/fmt/glaze_formatter.cpp
        src/base-common/fmt/glaze_formatter.hpp
        src/base-common/conversion/cw2a.hpp
        src/base-common/conversion/cw2a.cpp
        src/base-common/conversion/joaat.hpp
        src/base-common/util/ranged_value.hpp
        src/base-common/util/enum_bitops.hpp
)
add_library(base::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

include(vendor.cmake)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp)
endif ()

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_subdirectory(test)