#
# asjit - an AngelScript bytecode JIT, translating to AsmJit.
#
# Standalone on purpose. It depends on nothing in this monorepo, so it can be moved to its own
# repository and consumed from another project with add_subdirectory through CPM. This file is part
# of that promise: anything it reaches for outside its own directory is something that would have to
# be carried along.
#
cmake_minimum_required(VERSION 3.16)
project(asjit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(${PROJECT_NAME} STATIC
        src/pch.hpp
        src/abi.hpp
        src/bytecode.hpp
        src/emitter.hpp
        src/emitter.cpp
        src/runtime.hpp
        src/runtime.cpp
        src/function_compiler.hpp
        src/function_compiler.cpp
        src/jit_compiler.hpp
        src/jit_compiler.cpp
        src/asjit.hpp
        src/asjit.cpp
)
add_library(base::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# The monorepo's cmake_scripts/arch_macro.cmake and compiler_id_macro.cmake are deliberately not
# included here. They reach the project through ${CMAKE_SOURCE_DIR}, which is the monorepo root when
# this is configured as part of it and would not exist at all if asjit were configured on its own -
# the one include that would break the migration this component is written for. Nothing here used
# what they define: they added a bare `MSVC` and a bare host-architecture macro, and the only mention
# of either left in this component is in a comment.

# /bigobj: the emitter is one large translation unit of near-identical instruction cases.
# Third-party code is not built here, so none of these flags reach AsmJit or AngelScript.
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /wd4068 /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

include(vendor.cmake)

# Deliberately no dependency on base::common, unlike every other component here. The point of
# building the JIT as its own library is that it can be lifted into another repository and consumed
# with add_subdirectory through CPM, and a link to a target that only exists inside this monorepo
# would be the one thing that stops that working. Nothing in it needs logging: a JIT that cannot
# compile something declines it and lets the interpreter run, which is a normal outcome rather than a
# diagnostic, and the counters in asjit.hpp are what a caller reads instead.

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# A third suite is built but not listed here: AsmJit's own, reached through vendor.cmake, off by
# default and enabled with -DASJIT_BUILD_ASMJIT_TESTS=ON. Nothing here needs it to build, and it is
# the only way to check the pinned AsmJit rather than this component - `ctest -L asmjit` after such a
# configure.
add_subdirectory(support)
add_subdirectory(test)
add_subdirectory(bench)
