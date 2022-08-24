cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)
enable_language(ASM_MASM)

file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
)

include(FetchContent)

FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG f44fa31f5110331af196d0ed7f00ae1c7b8ef3cc)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(discord_rpc GIT_REPOSITORY https://github.com/X-rays5/discord-rpc.git GIT_TAG 963aa9f3e5ce81a4682c6ca3d136cddda614db33)
FetchContent_MakeAvailable(discord_rpc)

FetchContent_Declare(minhook GIT_REPOSITORY https://github.com/X-rays5/minhook.git GIT_TAG 4a455528f61b5a375b1f9d44e7d296d47f18bb18)
FetchContent_MakeAvailable(minhook)

FetchContent_Declare(robin_hood GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git GIT_TAG fb1483621fda28d4afb31c0097c1a4a457fdd35b)
FetchContent_MakeAvailable(robin_hood)

FetchContent_Declare(freetype GIT_REPOSITORY https://github.com/freetype/freetype.git GIT_TAG 919561ca8c2a7b245644767b49d08e638d34d4eb)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(gtav_classes GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes.git GIT_TAG 6edda15bb7a1eb47dc53ee9de555414e05e9ee00)
FetchContent_MakeAvailable(gtav_classes)

FetchContent_Declare(rapidjson GIT_REPOSITORY https://github.com/Tencent/rapidjson.git GIT_TAG 27c3a8dc0e2c9218fe94986d249a12b5ed838f1d)
FetchContent_MakeAvailable(rapidjson)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES} src/ui/fonts/fa.h src/ui/fonts/IconsFontAwesome6.h src/natives/invoker.asm src/dllmain.cpp src/main.cpp src/misc/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/misc/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/memory/sigscanner.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/scriptmanager/scriptmanager.cpp src/scriptmanager/scriptmanager.hpp src/scriptmanager/base_script.hpp src/scriptmanager/enums.hpp src/scripts/render/uidraw.cpp src/scripts/render/uidraw.hpp src/rpc/discord.cpp src/rpc/discord.hpp src/rage/enums.hpp src/rage/joaat.hpp src/d3d/renderer.cpp src/d3d/renderer.hpp src/ui/fonts/roboto_mono.hpp src/hooking/vmt.hpp src/hooking/detour.hpp src/misc/timedinput.cpp src/misc/timedinput.hpp src/d3d/texturemanager.cpp src/d3d/texturemanager.hpp src/d3d/draw.hpp src/scripts/scripting/discord.cpp src/scripts/scripting/discord.hpp src/ui/notification/notification.cpp src/ui/notification/notification.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/enums.hpp src/ui/components/submenu.hpp src/ui/components/options/baseoption.hpp src/ui/components/options/executeoption.hpp src/ui/components/options/labeloption.hpp src/ui/components/options/listoption.hpp src/ui/components/options/numberoption.hpp src/ui/components/options/submenuoption.hpp src/ui/components/options/toggleoption.hpp src/scripts/scripting/uitick.cpp src/scripts/scripting/uitick.hpp src/rage/util/execasscript.hpp src/rage/classes/script_thread.hpp src/rage/classes/fwddec.hpp src/rage/classes/tls_context.hpp src/rage/classes/script_handler.hpp src/rage/classes/script_id.hpp src/rage/classes/net_game_event.hpp src/rage/classes/at_array.hpp src/rage/classes/sys_mem_allocator.hpp src/rage/types.hpp src/rage/classes/scr_vector.hpp src/rage/classes/node_list.hpp src/hooking/detour.cpp src/hooking/vmt.cpp src/misc/common.cpp src/fiber/manager.cpp src/fiber/manager.hpp src/fiber/script.cpp src/fiber/script.hpp src/fiber/pool.cpp src/fiber/pool.hpp src/natives/crossmap.hpp src/natives/invoker.cpp src/natives/invoker.hpp src/rage/classes/natives.hpp src/natives/natives.hpp src/rage/classes/base.hpp src/ui/components/options/togglenumberoption.hpp src/logger/stacktrace.cpp src/logger/stacktrace.hpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)

target_include_directories(${PROJECT_NAME} PRIVATE ${discord_rpc_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${gtav_classes_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/xorstr/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/binary_bakery)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
