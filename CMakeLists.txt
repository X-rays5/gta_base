cmake_minimum_required(VERSION 3.16)
project(gta_base)

include(cmake_scripts/get_cpm.cmake)

if (WIN32)
  if (MSVC)
    add_subdirectory(menu)
  endif ()
endif ()