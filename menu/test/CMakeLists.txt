cmake_minimum_required(VERSION 3.26)
project(menu_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE MENU_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp)
list(REMOVE_ITEM MENU_CPP_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dllmain.cpp
)

file(GLOB_RECURSE MENU_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.hpp)

add_executable(${PROJECT_NAME}
        ${MENU_CPP_FILES}
        ${MENU_HEADER_FILES}
        src/main.cpp
)

include(../../vendor/gtest.cmake)
enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})