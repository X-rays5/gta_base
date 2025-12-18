cmake_minimum_required(VERSION 3.26)
project(injector CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Core library containing all source files except main.cpp
# This allows sharing code between the main executable and tests
add_library(${PROJECT_NAME}_core OBJECT
        src/window.cpp
        src/window.hpp
        src/settings.cpp
        src/settings.hpp
        src/inject.cpp
        src/inject.hpp
)
add_library(base::${PROJECT_NAME}_core ALIAS ${PROJECT_NAME}_core)

# Main executable that uses the core library
add_executable(${PROJECT_NAME}
        src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_core)
target_compile_definitions(${PROJECT_NAME} PUBLIC BASE_SUBCOMPONENT="${PROJECT_NAME}")

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

include(vendor.cmake)

# Configure core library
target_compile_definitions(${PROJECT_NAME}_core PUBLIC BASE_SUBCOMPONENT="${PROJECT_NAME}")
set_property(TARGET ${PROJECT_NAME}_core PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME}_core PRIVATE /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME}_core PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME}_core PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME}_core PUBLIC base::common)
target_link_libraries(${PROJECT_NAME}_core PUBLIC base::imfont)

# Configure main executable
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/menu_bin")
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY WIN32_EXECUTABLE TRUE)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} REUSE_FROM ${PROJECT_NAME}_core)

add_subdirectory(test)