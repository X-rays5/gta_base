CPMAddPackage(
        NAME LuaJit
        GITHUB_REPOSITORY "LuaJIT/LuaJIT"
        GIT_TAG "c6ffc141a8762b41703f9287d63d93622a13dd8f"
        DOWNLOAD_ONLY YES
)

if (NOT ${LuaJit_ADDED})
    message(FATAL_ERROR "Failed to add LuaJit")
endif ()

set(LUAJIT_DIR ${LuaJit_SOURCE_DIR})
set(LUAJIT_BUILD_EXE OFF CACHE BOOL "" FORCE)

enable_language(ASM_MASM)
CPMAddPackage("gh:X-rays5/luajit-cmake#714b60a81b76de981f2ec80a77ba1d0a21c0e11a")

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
target_compile_definitions(${PROJECT_NAME} PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_STRINGS_ARE_NUMBERS=1 SOL_LUAJIT=1 SOL_NO_EXCEPTIONS=1 SOL_PRINT_ERRORS=0)