#
# asjit_bench - measures whether the JIT is worth having, per workload class.
#
# Not a test and not registered with ctest. A timing run belongs to a human deciding something, not
# to CI, where the numbers would be noise from whatever else the runner was doing. It is built by
# the default target so that it cannot rot silently.
#
cmake_minimum_required(VERSION 3.16)
project(asjit_bench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/workloads.hpp
        src/workloads.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE base::asjit_support)

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

# Deliberately NOT a configure-time error on a Debug build, which would be the tidier place to catch
# it. The library is built in Debug for everyday development, and failing the configure would mean
# adding a benchmark target broke everyone's working build directory. The refusal happens at run
# time instead - see main.cpp - which costs a wasted compile and buys a Debug tree that still works.
