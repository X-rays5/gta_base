cmake_minimum_required(VERSION 3.26)
project(minicoropp_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/coroutine_test.cpp
)

include(../../vendor/gtest.cmake)
enable_testing()

target_link_libraries(${PROJECT_NAME} PRIVATE base::minicoropp)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
