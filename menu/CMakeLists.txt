cmake_minimum_required(VERSION 3.25)
project(menu)

set(CMAKE_CXX_STANDARD 23)
enable_language(ASM_MASM)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
# turn on the experimental API
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")

add_library(${PROJECT_NAME} MODULE src/dllmain.cpp src/main.cpp src/main.hpp)
target_sources(${PROJECT_NAME} PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES FILES src/util/fs.ixx)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

include(${CMAKE_SOURCE_DIR}/cmake_scripts/arch_macro.cmake)
include(${CMAKE_SOURCE_DIR}/cmake_scripts/compiler_id_macro.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /wd4068 /diagnostics:caret)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)