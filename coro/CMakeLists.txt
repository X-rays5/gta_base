cmake_minimum_required(VERSION 3.16)
project(minicoropp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

option(MINICOROPP_USE_ASM "Use ASM implementation for coroutine context switching" ON)
option(MINICOROPP_NO_MULTITHREAD "Disable thread-local storage for minicoropp" OFF)
option(MINICOROPP_NO_DEFAULT_ALLOCATOR "Disable default mco allocator" OFF)
option(MINICOROPP_USE_VMEM_ALLOCATOR "Use Virtual Memory allocator instead of malloc" OFF)

set(MINICOROPP_MIN_STACK_SIZE 32768 CACHE STRING "Minimum stack size for coroutines")
set(MINICOROPP_DEFAULT_STACK_SIZE 65536 CACHE STRING "Default stack size for coroutines (if not using vmem allocator)")
set(MINICOROPP_DEFAULT_STORAGE_SIZE 1024 CACHE STRING "Default storage size reserved for coroutine buffers")

# Create minicoro_config.h
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/minicoro_config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/base-coro/minicoro_config.h
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)

# Copy headers to the include directory
foreach (header ${HEADERS})
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR}/src ${header})
    set(dest ${CMAKE_CURRENT_BINARY_DIR}/include/base-coro/${rel_path})
    configure_file(${header} ${dest} COPYONLY)
endforeach ()

add_library(${PROJECT_NAME} STATIC
        src/coroutine.cpp
        src/coroutine.hpp
        src/mutex.cpp
        src/mutex.hpp
)
add_library(base::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

include(vendor.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE minicoro::minicoro)

# We make sure the headers are updated every build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/base-coro)
foreach (header ${HEADERS})
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/src" ${header})
    get_filename_component(dir_path ${rel_path} DIRECTORY)

    # Create directory if needed
    if (dir_path)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/base-coro/${dir_path})
    endif ()

    # Single copy command for all cases
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${header}
            ${CMAKE_CURRENT_BINARY_DIR}/include/base-coro/${rel_path}
            COMMENT "Copying ${rel_path} to include directory"
    )
endforeach ()

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(test)
