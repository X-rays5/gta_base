cmake_minimum_required(VERSION 3.26)
project(imfont_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/main.cpp
        src/imfont_test.cpp
        src/test_globals.hpp
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

include(../../vendor/battery_embed.cmake)

include(../../vendor/gtest.cmake)
enable_testing()

target_link_libraries(${PROJECT_NAME} PRIVATE base::imfont)
target_link_libraries(${PROJECT_NAME} PRIVATE base::common)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})