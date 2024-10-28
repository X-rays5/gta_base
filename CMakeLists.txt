cmake_minimum_required(VERSION 3.16)
project(gta_base)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake_scripts/get_cpm.cmake)

if (WIN32)
  if (MSVC)
    add_subdirectory(menu)
  endif ()
endif ()