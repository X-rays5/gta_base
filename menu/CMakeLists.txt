cmake_minimum_required(VERSION 3.26)
project(menu CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

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
        src/logging/exception/exception_report.cpp
        src/logging/exception/exception_report.hpp
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
        src/hooking/hooking.cpp
        src/hooking/hooking.hpp
        src/hooking/helpers/detour.cpp
        src/hooking/helpers/detour.hpp
        src/hooking/helpers/vmt.cpp
        src/hooking/helpers/vmt.hpp
        src/memory/pointers.cpp
        src/memory/pointers.hpp
        src/win32/signal.hpp
        src/logging/formatter/thread_id.hpp
        src/render/font/manager.cpp
        src/render/font/manager.hpp
        src/render/font/IconsFontAwesome6.hpp
        src/ui/localization/manager.cpp
        src/ui/localization/manager.hpp
        src/ui/localization/default.cpp
        src/util/glaze/glaze_meta.cpp
        src/util/glaze/glaze_meta.hpp
        src/util/result.hpp
        src/util/type_traits.hpp
        src/util/fmt/formatter.cpp
        src/util/fmt/formatter.hpp
        src/logging/plh_logger.hpp
        src/memory/signature/pattern.cpp
        src/memory/signature/pattern.hpp
        src/memory/address.hpp
        src/memory/signature/batch.cpp
        src/memory/signature/batch.hpp
        src/util/thread_pool.hpp
        src/discord/rich_presence.cpp
        src/discord/rich_presence.hpp
        src/ui/notification/notification.cpp
        src/ui/notification/notification.hpp
        src/ui/notification/manager.cpp
        src/ui/notification/manager.hpp
        src/render/animate.hpp
        src/util/profile.hpp
        src/util/string.cpp
        src/util/string.hpp
)

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

include(vendor.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/menu_dll")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp)

b_embed(${PROJECT_NAME} assets/fonts/fa-solid-900.ttf)
b_embed(${PROJECT_NAME} assets/fonts/RobotoMono-Regular.ttf)