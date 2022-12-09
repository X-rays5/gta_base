CPMAddPackage(
        NAME lua
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.4
        DOWNLOAD_ONLY YES
)

if (lua_ADDED)
    FILE(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
    list(REMOVE_ITEM lua_sources "${lua_SOURCE_DIR}/lua.c" "${lua_SOURCE_DIR}/luac.c")
    add_library(lua STATIC ${lua_sources})

    target_include_directories(lua PUBLIC $<BUILD_INTERFACE:${lua_SOURCE_DIR}>)

    CPMAddPackage(
            NAME sol2
            GIT_REPOSITORY https://github.com/ThePhD/sol2.git
            GIT_TAG v3.3.0
    )

    if (NOT sol2_ADDED)
        message(FATAL_ERROR "Failed to add sol2")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${sol2_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} PRIVATE lua)

    target_compile_definitions(${PROJECT_NAME} PRIVATE SOL_ALL_SAFETIES_ON SOL_DEFAULT_PASS_ON_ERROR SOL_STRINGS_ARE_NUMBERS)

else()
    message(FATAL_ERROR "Failed to add lua")
endif()