cmake_minimum_required(VERSION 3.26)
project(injector_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE INJECTOR_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}../src/*.cpp)
list(REMOVE_ITEM INJECTOR_CPP_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

file(GLOB_RECURSE INJECTOR_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}../src/*.hpp)

add_executable(${PROJECT_NAME}
        ${INJECTOR_CPP_FILES}
        ${INJECTOR_HEADER_FILES}
        src/main.cpp
)

include(../../vendor/gtest.cmake)
enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})