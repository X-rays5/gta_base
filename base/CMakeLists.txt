cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(discord_rpc GIT_REPOSITORY https://github.com/X-rays5/discord-rpc.git GIT_TAG 963aa9f3e5ce81a4682c6ca3d136cddda614db33)
FetchContent_MakeAvailable(discord_rpc)

FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/X-rays5/spdlog.git GIT_TAG 100f30043f33277122e0991c83845a2617172ffd)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(minhook GIT_REPOSITORY https://github.com/X-rays5/minhook.git GIT_TAG 4a455528f61b5a375b1f9d44e7d296d47f18bb18)
FetchContent_MakeAvailable(minhook)

add_library(${PROJECT_NAME} SHARED src/dllmain.cpp src/main.cpp src/common/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/common/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/hooking/minhook/minhook.cpp src/hooking/minhook/minhook.hpp src/memory/sigscanner.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/threads/threads.cpp src/threads/threads.hpp src/threads/base_script.hpp src/threads/threaddefs.hpp src/scripts/game_main/game_script.cpp src/scripts/game_main/game_script.hpp src/scripts/scripting_main/main_script.cpp src/scripts/scripting_main/main_script.hpp src/scripts/render_main/render_script.cpp src/scripts/render_main/render_script.hpp src/rpc/discord.cpp src/rpc/discord.hpp src/rage/enums.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${discord_rpc_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook)
