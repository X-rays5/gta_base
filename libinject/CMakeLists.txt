cmake_minimum_required(VERSION 3.16)
project(libinject)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC src/main.cpp)