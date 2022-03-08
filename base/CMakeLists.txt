cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/minhook)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/discord-rpc)

add_library(${PROJECT_NAME} SHARED src/dllmain.cpp src/main.cpp src/common/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/common/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/hooking/minhook/minhook.cpp src/hooking/minhook/minhook.hpp src/memory/sigscanner.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/threads/threads.cpp src/threads/threads.hpp src/threads/base_script.hpp src/threads/threaddefs.hpp src/scripts/game_main/game_script.cpp src/scripts/game_main/game_script.hpp src/scripts/scripting_main/main_script.cpp src/scripts/scripting_main/main_script.hpp src/scripts/render_main/render_script.cpp src/scripts/render_main/render_script.hpp src/rpc/discord.cpp src/rpc/discord.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/discord-rpc/include)

target_link_libraries(${PROJECT_NAME} PRIVATE minhook)
target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)
