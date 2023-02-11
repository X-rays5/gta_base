cmake_minimum_required(VERSION 3.16)
project(gta_base)

include(get_cpm.cmake)

if (WIN32)
    if (MSVC)
        add_subdirectory(base)
    endif ()
    add_subdirectory(launcher)
endif ()