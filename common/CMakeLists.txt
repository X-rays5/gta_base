cmake_minimum_required(VERSION 3.16)
project(common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)

# Copy headers to the include directory
foreach (header ${HEADERS})
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR}/src ${header})
    set(dest ${CMAKE_CURRENT_BINARY_DIR}/include/base-common/${rel_path})
    configure_file(${header} ${dest} COPYONLY)
endforeach ()

add_library(${PROJECT_NAME} STATIC
        src/globals.hpp
        src/concurrency/spinlock.cpp
        src/concurrency/spinlock.hpp
        src/util/result.cpp
        src/util/result.hpp
        src/fmt/fs_formatter.cpp
        src/fmt/fs_formatter.hpp
        src/util/string.cpp
        src/util/string.hpp
        src/util/time.cpp
        src/util/time.hpp
        src/conversion/hex.cpp
        src/conversion/hex.hpp
        src/logging/logger.hpp
        src/logging/logger.cpp
        src/logging/logging_macro.hpp
        src/logging/exception/exception_report.cpp
        src/logging/exception/exception_report.hpp
        src/logging/exception/util.hpp
        src/logging/exception/util.cpp
        src/logging/exception/vectored_handler.hpp
        src/logging/exception/vectored_handler.cpp
        src/logging/formatter/thread_id.hpp
        src/win32/all.hpp
        src/win32/memory.hpp
        src/win32/memory.cpp
        src/win32/misc.hpp
        src/win32/misc.cpp
        src/win32/signal.hpp
        src/fs/vfs.cpp
        src/fs/vfs.hpp
        src/memory/address.cpp
        src/memory/address.hpp
        src/fmt/glaze_formatter.cpp
        src/fmt/glaze_formatter.hpp
        src/conversion/cw2a.hpp
        src/conversion/cw2a.cpp
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

# We make sure the headers are updated every build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/base-common)
foreach (header ${HEADERS})
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/src" ${header})
    get_filename_component(dir_path ${rel_path} DIRECTORY)

    # Create directory if needed
    if (dir_path)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/base-common/${dir_path})
    endif ()

    # Single copy command for all cases
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${header}
            ${CMAKE_CURRENT_BINARY_DIR}/include/base-common/${rel_path}
            COMMENT "Copying ${rel_path} to include directory"
    )
endforeach ()

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(test)