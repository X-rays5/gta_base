cmake_minimum_required(VERSION 3.16)
project(lua_codegen)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp)