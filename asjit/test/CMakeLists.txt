cmake_minimum_required(VERSION 3.16)
project(asjit_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(${PROJECT_NAME}
        src/test_support.hpp
        src/test_support.cpp
        src/differential_test.cpp
        src/compiler_test.cpp
        src/emitter_test.cpp
)

# AngelScript and AsmJit arrive through base::asjit, so nothing is vendored here. The add-on headers
# in particular - scriptbuilder, scriptstdstring, scriptarray - come from the include directory the
# engine's own build produces, which is why they are reachable as <angelscript/...>.
include(../../vendor/gtest.cmake)

# base::asjit_support, not base::asjit: the engine harness is shared with the benchmark so that the
# two cannot disagree about what a correctly configured engine is. It brings asjit, AngelScript and
# the add-on headers with it.
target_link_libraries(${PROJECT_NAME} PRIVATE base::asjit_support)

# AsmJit as well, which the library deliberately keeps to itself: `emitter.hpp` is what the emitter
# tests include, so they assemble instructions directly, and an assembler needs the headers of the
# assembler library. Naming it here rather than making it PUBLIC on base::asjit is the same decision
# the library's own vendor.cmake records - menu_core already carries PolyHook's AsmJit on its include
# path, and a second AsmJit include root reaching it would make those includes ambiguous. A test
# target that links nothing else does not have that problem.
target_link_libraries(${PROJECT_NAME} PRIVATE asjit::asmjit)

enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
