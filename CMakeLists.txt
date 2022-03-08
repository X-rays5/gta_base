cmake_minimum_required(VERSION 3.16)
project(gta_base)

IF(WIN32)
    IF(MSVC)
        add_subdirectory(base)
    ENDIF()
    add_subdirectory(launcher)
ENDIF()

add_subdirectory(lua_codegen)