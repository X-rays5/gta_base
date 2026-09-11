cmake_minimum_required(VERSION 3.26)
project(menu_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/as_docs_test.cpp
        src/as_natives_test.cpp
)

# Link against the core library to share dependencies and avoid recompilation
# The add-ons are linked explicitly because the core library keeps AngelScript private, and the
# tests drive the engine directly.
target_link_libraries(${PROJECT_NAME} PRIVATE base::menu_core angelscript_add_ons)

include(../../vendor/gtest.cmake)
enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})