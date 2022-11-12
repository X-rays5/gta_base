cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)
enable_language(ASM_MASM)

file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
)

CPMAddPackage("gh:gabime/spdlog#77429b2e2e70feb5d2619374f2bc99326f6570dd")
CPMAddPackage("gh:X-rays5/minhook#7f3cae16668187282f0a64df8d2918cd0e51258f")
CPMAddPackage(NAME robin_hood GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git GIT_TAG fb1483621fda28d4afb31c0097c1a4a457fdd35b)
CPMAddPackage("gh:freetype/freetype#c943d408e08fbe6d3e12f647b4a10d94a8198e9b")
CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage("gh:X-rays5/GTAV-Classes#c648856759bfe788a40fd9b12fbe02e16f871225")
CPMAddPackage("gh:Tencent/rapidjson#27c3a8dc0e2c9218fe94986d249a12b5ed838f1d")
CPMAddPackage("gh:Neargye/magic_enum#159a35006d8e18172eb687b432b77e6f5baa61f6")
CPMAddPackage("gh:zeux/pugixml#v1.13")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/discord-rpc)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES} src/natives/spoof.asm src/ui/translation/translation_manager.cpp src/ui/translation/translation_manager.hpp src/ui/fonts/IconsFontAwesome6.h src/dllmain.cpp src/main.cpp src/misc/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/misc/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/scriptmanager/scriptmanager.cpp src/scriptmanager/scriptmanager.hpp src/scriptmanager/base_script.hpp src/scriptmanager/enums.hpp src/scripts/render/uidraw.cpp src/scripts/render/uidraw.hpp src/rpc/discord.cpp src/rpc/discord.hpp src/rage/enums.hpp src/rage/joaat.hpp src/d3d/renderer.cpp src/d3d/renderer.hpp src/hooking/hooking_helpers/vmt.hpp src/hooking/hooking_helpers/detour.hpp src/misc/timedinput.cpp src/misc/timedinput.hpp src/d3d/draw.hpp src/ui/notification/notification.cpp src/ui/notification/notification.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/enums.hpp src/ui/components/submenu.hpp src/ui/components/options/baseoption.hpp src/ui/components/options/executeoption.hpp src/ui/components/options/labeloption.hpp src/ui/components/options/listoption.hpp src/ui/components/options/numberoption.hpp src/ui/components/options/submenuoption.hpp src/ui/components/options/toggleoption.hpp src/scripts/scripting/uitick.cpp src/scripts/scripting/uitick.hpp src/rage/util/exec_as_script.hpp src/rage/classes/script_thread.hpp src/rage/classes/fwddec.hpp src/rage/classes/tls_context.hpp src/rage/classes/script_handler.hpp src/rage/classes/script_id.hpp src/rage/classes/net_game_event.hpp src/rage/classes/at_array.hpp src/rage/classes/sys_mem_allocator.hpp src/rage/types.hpp src/rage/classes/scr_vector.hpp src/rage/classes/node_list.hpp src/hooking/hooking_helpers/detour.cpp src/hooking/hooking_helpers/vmt.cpp src/misc/common.cpp src/fiber/manager.cpp src/fiber/manager.hpp src/fiber/script.cpp src/fiber/script.hpp src/fiber/pool.cpp src/fiber/pool.hpp src/natives/crossmap.hpp src/natives/invoker.cpp src/natives/invoker.hpp src/rage/classes/natives.hpp src/natives/natives.hpp src/rage/classes/base.hpp src/ui/components/options/togglenumberoption.hpp src/logger/stacktrace.cpp src/logger/stacktrace.hpp src/scripts/game/ui_disable_control.cpp src/scripts/game/ui_disable_control.hpp src/ui/components/options/togglelistoption.hpp src/memory/scanner/all.hpp src/memory/scanner/handle.hpp src/memory/scanner/module.cpp src/memory/scanner/module.hpp src/memory/scanner/pattern.cpp src/memory/scanner/pattern.hpp src/memory/scanner/range.cpp src/memory/scanner/range.hpp src/memory/scanner/sig_batch_scan.cpp src/memory/scanner/sig_batch_scan.hpp src/memory/scanner/fwdec.hpp src/rage/util/get.hpp src/rage/classes/net_connection_mgr.hpp src/rage/classes/net_connection_peer.hpp src/rage/util/breakup_kick.hpp src/ui/translation/translation_manager.cpp src/ui/translation/translation_manager.hpp src/player_mgr/player.cpp src/player_mgr/player.hpp src/player_mgr/manager.cpp src/player_mgr/manager.hpp src/hooking/hooked_fn_impl/player_join_leave/network_player_mgr.cpp src/hooking/hooked_fn_impl/player_join_leave/assign_physical_idx.cpp src/ui/components/keyboard.hpp src/scripts/scripting/job_queue.cpp src/scripts/scripting/job_queue.hpp src/scripts/game/loops.cpp src/scripts/game/loops.hpp src/looped_game/self_globals.cpp src/looped_game/self_globals.hpp src/looped_game/self_features.cpp src/looped_game/self_features.hpp src/misc/settings.hpp src/misc/settings.cpp src/rage/util/teleport.cpp src/rage/util/teleport.hpp src/rage/util/blip.cpp src/rage/util/blip.hpp src/rage/util/streaming.cpp src/rage/util/streaming.hpp src/ui/tabs/main_tab.cpp src/ui/tabs/main_tab.hpp src/ui/tabs/selected_player.cpp src/ui/tabs/selected_player.hpp src/ui/tabs/settings.cpp src/ui/tabs/settings.hpp src/ui/tabs/misc.cpp src/ui/tabs/misc.hpp src/ui/tabs/world.cpp src/ui/tabs/world.hpp src/ui/tabs/self.cpp src/ui/tabs/self.hpp src/ui/tabs/debug.cpp src/ui/tabs/debug.hpp src/ui/tabs/tab_includes.hpp src/rage/util/entity.cpp src/rage/util/entity.hpp src/ui/tabs/weapon.cpp src/ui/tabs/weapon.hpp src/looped_game/weapon.cpp src/looped_game/weapon.hpp src/rage/script/global.cpp src/rage/script/global.hpp src/rage/script/local.cpp src/rage/script/local.hpp src/rage/util/session_switcher.cpp src/rage/util/session_switcher.hpp src/rage/global_idx.hpp src/ui/tabs/network.cpp src/ui/tabs/network.hpp src/ui/tabs/all_players.cpp src/ui/tabs/all_players.hpp src/looped_game/transition_state.cpp src/looped_game/transition_state.hpp src/misc/hotkey_manager.cpp src/misc/hotkey_manager.hpp src/ui/components/keyboard.cpp src/hooking/native_hooking.cpp src/hooking/native_hooking.hpp src/hooking/hooking_helpers/script.cpp src/hooking/hooking_helpers/script.hpp src/rage/classes/script_program.hpp src/rage/util/find_script_thread.hpp src/misc/json.cpp src/misc/json.hpp src/misc/thread_pool.cpp src/misc/thread_pool.hpp src/rage/data/data_loader.cpp src/rage/data/data_loader.hpp src/rage/data/data_manager.cpp src/rage/data/data_manager.hpp src/rage/util/fipack_reader.cpp src/rage/util/fipack_reader.hpp src/rage/classes/fidevice.cpp src/rage/classes/fidevice.hpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)

target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${magic_enum_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/xorstr/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/binary_bakery)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/discord-rpc/include)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
