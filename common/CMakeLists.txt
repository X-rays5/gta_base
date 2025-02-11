cmake_minimum_required(VERSION 3.16)
project(common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)
file(COPY ${HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/base-common)

add_library(${PROJECT_NAME} STATIC
    src/globals.hpp
        src/util/spinlock.cpp
        src/util/spinlock.hpp
        src/result.cpp
        src/result.hpp
        src/fmt.cpp
        src/fmt.hpp
        src/util/string.cpp
        src/util/string.hpp
        src/util/time.cpp
        src/util/time.hpp
        src/conversion.cpp
        src/conversion.hpp
)
add_library(base::common ALIAS ${PROJECT_NAME})

include(vendor.cmake)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)