CPMAddPackage(
        NAME LuaJit
        GITHUB_REPOSITORY "LuaJIT/LuaJIT"
        GIT_TAG "18b087cd2cd4ddc4a79782bf155383a689d5093d"
        DOWNLOAD_ONLY YES
)

if (NOT ${LuaJit_ADDED})
    message(FATAL_ERROR "Failed to add LuaJit")
endif ()

set(LUAJIT_DIR ${LuaJit_SOURCE_DIR})
set(LUAJIT_BUILD_EXE OFF CACHE BOOL "" FORCE)

enable_language(ASM_MASM)
CPMAddPackage("gh:X-rays5/luajit-cmake#559c8126a99fd60b4df4e99322f4cd2634d1e1b1")

if (NOT ${luajit-cmake_ADDED})
    message(FATAL_ERROR "Failed to add luajit-cmake")
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/luajit-cmake-build)

CPMAddPackage(
        NAME sol2
        GITHUB_REPOSITORY "ThePhD/sol2"
        GIT_TAG "c1f95a773c6f8f4fde8ca3efe872e7286afe4444"
        DOWNLOAD_ONLY YES
)

if (NOT sol2_ADDED)
    message(FATAL_ERROR "Failed to add sol2")
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE libluajit)
target_include_directories(${PROJECT_NAME} PRIVATE ${sol2_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_STRINGS_ARE_NUMBERS=1 SOL_LUAJIT=1)