cmake_minimum_required(VERSION 3.26)
project(injector CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(${PROJECT_NAME}
        src/main.cpp)

include(vendor.cmake)

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME} PRIVATE base::common)