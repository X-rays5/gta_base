cmake_minimum_required(VERSION 3.26)
project(menu CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} MODULE src/dllmain.cpp
            src/main.cpp
            src/main.hpp
            src/render/draw.cpp
            src/render/draw.hpp
            src/render/manager.cpp
            src/render/manager.hpp
            src/render/render_hook.cpp
            src/render/render_hook.hpp
            src/logging/logger.cpp
            src/logging/logger.hpp
            src/logging/logging_macro.hpp
            src/util/vfs.hpp
            src/win32/all.hpp
            src/win32/misc.cpp
            src/win32/misc.hpp
            src/globals.hpp
            src/util/common.cpp
            src/util/common.hpp
            src/win32/memory.cpp
            src/win32/memory.hpp
)

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

include(../vendor/fmt.cmake)
include(../vendor/spdlog.cmake)
include(../vendor/xorstr.cmake)
include(../vendor/robin_hood.cmake)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /wd4068 /diagnostics:caret)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)