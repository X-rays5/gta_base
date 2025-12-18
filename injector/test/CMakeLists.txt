cmake_minimum_required(VERSION 3.26)
project(injector_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
)

# Link against the core library to share dependencies and avoid recompilation
target_link_libraries(${PROJECT_NAME} PRIVATE base::injector_core)

include(../../vendor/gtest.cmake)
enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})