cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)

file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
)


include(FetchContent)

FetchContent_Declare(discord_rpc GIT_REPOSITORY https://github.com/X-rays5/discord-rpc.git GIT_TAG 963aa9f3e5ce81a4682c6ca3d136cddda614db33)
FetchContent_MakeAvailable(discord_rpc)

FetchContent_Declare(g3log GIT_REPOSITORY https://github.com/X-rays5/g3log.git GIT_TAG a5a48f8962ddff000de28ede16b1c06f60f92409)
set(G3_SHARED_LIB OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(g3log)

FetchContent_Declare(minhook GIT_REPOSITORY https://github.com/X-rays5/minhook.git GIT_TAG 4a455528f61b5a375b1f9d44e7d296d47f18bb18)
FetchContent_MakeAvailable(minhook)

FetchContent_Declare(robin_hood GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git GIT_TAG fb1483621fda28d4afb31c0097c1a4a457fdd35b)
FetchContent_MakeAvailable(robin_hood)

FetchContent_Declare(freetype GIT_REPOSITORY https://github.com/freetype/freetype.git GIT_TAG 919561ca8c2a7b245644767b49d08e638d34d4eb)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 7bdf0628b1276379886c7f6dda2cef2b3b374f0b)
FetchContent_MakeAvailable(fmt)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES} src/dllmain.cpp src/main.cpp src/common/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/common/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/memory/sigscanner.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/threads/threads.cpp src/threads/threads.hpp src/threads/base_script.hpp src/threads/threaddefs.hpp src/scripts/game_main/game_script.cpp src/scripts/game_main/game_script.hpp src/scripts/scripting_main/main_script.cpp src/scripts/scripting_main/main_script.hpp src/scripts/render_main/render_script.cpp src/scripts/render_main/render_script.hpp src/rpc/discord.cpp src/rpc/discord.hpp src/rage/enums.hpp src/rage/joaat.hpp src/rage/classes/fwddec.hpp src/rage/classes/atSingleton.hpp src/rage/classes/CAmmoInfo.hpp src/rage/classes/CAutomobile.hpp src/rage/classes/CBaseModelInfo.hpp src/rage/classes/CHandlingData.hpp src/rage/classes/CItemInfo.hpp src/rage/classes/CNavigation.hpp src/rage/classes/CNetGamePlayer.hpp src/rage/classes/CNetworkPlayerMgr.hpp src/rage/classes/CNonPhysicalPlayerData.hpp src/rage/classes/CPed.hpp src/rage/classes/CPedAppearanceDataNode.hpp src/rage/classes/CPedComponents.hpp src/rage/classes/CPedFactory.hpp src/rage/classes/CPedModelInfo.hpp src/rage/classes/CPedWeaponManager.hpp src/rage/classes/CPlayerAppearanceDataNode.hpp src/rage/classes/CPlayerInfo.hpp src/rage/classes/CTunables.hpp src/rage/classes/CVehicleDrawHandler.hpp src/rage/classes/CVehicleModelInfo.hpp src/rage/classes/CWeaponInfo.hpp src/rage/classes/FriendInfo.hpp src/rage/classes/FriendRegistry.hpp src/rage/classes/fwDrawData.hpp src/rage/classes/fwEntity.hpp src/rage/classes/HashTable.hpp src/rage/classes/netObject.hpp src/rage/classes/netPlayer.hpp src/rage/classes/netPlayerData.hpp src/rage/classes/ScInfo.hpp src/rage/classes/vector.hpp src/rage/classes.hpp src/d3d/renderer.cpp src/d3d/renderer.hpp src/ui/fonts/roboto_mono.hpp src/hooking/vmt.hpp src/hooking/detour.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${discord_rpc_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)

target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)
target_link_libraries(${PROJECT_NAME} PRIVATE g3log)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
