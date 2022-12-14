cmake_minimum_required(VERSION 3.16)
project(gta_base)

include(get_cpm.cmake)

IF(WIN32)
    IF(MSVC)
        add_subdirectory(base)
    ENDIF()
    add_subdirectory(crossmap_gen)
    add_subdirectory(launcher)
ENDIF()