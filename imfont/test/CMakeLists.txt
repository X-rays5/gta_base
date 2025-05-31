cmake_minimum_required(VERSION 3.26)
project(imfont_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/main.cpp
        src/imfont_test.cpp
        src/test_globals.hpp
)

include(../../vendor/battery_embed.cmake)
include(../../vendor/sdl.cmake)
include(../../vendor/imgui.cmake)
target_link_libraries(imgui PRIVATE SDL3::SDL3)

include(../../vendor/gtest.cmake)
enable_testing()

target_link_libraries(${PROJECT_NAME} PRIVATE base::imfont)
target_link_libraries(${PROJECT_NAME} PRIVATE base::common)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})