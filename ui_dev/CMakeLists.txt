cmake_minimum_required(VERSION 3.16)
project(ui_dev)

set(CMAKE_CXX_STANDARD 20)

add_executable(ui_dev WIN32 src/main.cpp src/wndproc.cpp src/wndproc.hpp src/d3d11/D3D11.cpp src/d3d11/D3D11.h)