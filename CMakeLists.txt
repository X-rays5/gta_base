cmake_minimum_required(VERSION 3.16)
project(gta_base)

IF(WIN32)
    add_subdirectory(base)
    add_subdirectory(libinject)
    add_subdirectory(launcher)
ENDIF()

add_subdirectory(lua_codegen)