cmake_minimum_required(VERSION 3.26)
project(menu CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(${PROJECT_NAME} MODULE src/dllmain.cpp
            src/main.cpp
            src/main.hpp
            src/render/draw.hpp
            src/render/renderer.cpp
            src/render/renderer.hpp
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
            src/logging/exception/stacktrace.cpp
            src/logging/exception/stacktrace.hpp
            src/logging/exception/vectored_handler.cpp
            src/logging/exception/vectored_handler.hpp
            src/win32/memory.cpp
            src/win32/memory.hpp
            src/logging/exception/util.hpp
            src/logging/exception/util.cpp
            src/render/draw_util.cpp
            src/render/draw_util.hpp
            src/render/draw_commands.hpp
            src/util/spinlock.cpp
            src/util/spinlock.hpp
            src/render/thread.cpp
            src/render/thread.hpp
            src/memory/scanner/all.hpp
            src/memory/scanner/fwdec.hpp
            src/memory/scanner/handle.hpp
            src/memory/scanner/module.cpp
            src/memory/scanner/module.hpp
            src/memory/scanner/pattern.cpp
            src/memory/scanner/pattern.hpp
            src/memory/scanner/range.cpp
            src/memory/scanner/range.hpp
            src/memory/scanner/sig_batch_scan.cpp
            src/memory/scanner/sig_batch_scan.hpp
            src/hooking/hooking.cpp
            src/hooking/hooking.hpp
            src/hooking/helpers/detour.cpp
            src/hooking/helpers/detour.hpp
            src/hooking/helpers/vmt.cpp
            src/hooking/helpers/vmt.hpp
            src/memory/pointers.cpp
            src/memory/pointers.hpp
            src/win32/signal.hpp
)

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

include(../vendor/fmt.cmake)
include(../vendor/spdlog.cmake)
include(../vendor/xorstr.cmake)
include(../vendor/robin_hood.cmake)
include(../vendor/imgui.cmake)
include(../vendor/leopard.cmake)
include(../vendor/abseil.cmake)
include(../vendor/magic_enum.cmake)
include(../vendor/inireader.cmake)
include(../vendor/minhook.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
