cmake_minimum_required(VERSION 3.21)
project(base)

set(CMAKE_CXX_STANDARD 20)
enable_language(ASM_MASM)

file(GLOB IMGUI_SOURCES
    "../vendor/imgui/*.hpp"
    "../vendor/imgui/*.cpp"
)

add_subdirectory(../vendor/discord-rpc ${CMAKE_CURRENT_BINARY_DIR}/discord-rpc)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES} src/natives/spoof.asm src/ui/translation/translation_manager.cpp src/ui/translation/translation_manager.hpp src/ui/fonts/IconsFontAwesome6.h src/dllmain.cpp src/main.cpp src/misc/globals.hpp src/main.hpp src/logger/logger.cpp src/logger/logger.hpp src/misc/common.hpp src/hooking/wndproc.cpp src/hooking/wndproc.hpp src/hooking/hooking.cpp src/hooking/hooking.hpp src/memory/pointers.cpp src/memory/pointers.hpp src/scriptmanager/scriptmanager.cpp src/scriptmanager/scriptmanager.hpp src/scriptmanager/base_script.hpp src/scriptmanager/enums.hpp src/scripts/render/uidraw.cpp src/scripts/render/uidraw.hpp src/rpc/discord.cpp src/rpc/discord.hpp src/rage/enums.hpp src/rage/joaat.hpp src/d3d/renderer.cpp src/d3d/renderer.hpp src/hooking/hooking_helpers/vmt.hpp src/hooking/hooking_helpers/detour.hpp src/misc/timedinput.cpp src/misc/timedinput.hpp src/d3d/draw.hpp src/ui/notification/notification.cpp src/ui/notification/notification.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/enums.hpp src/ui/components/submenu.hpp src/ui/components/options/baseoption.hpp src/ui/components/options/executeoption.hpp src/ui/components/options/labeloption.hpp src/ui/components/options/listoption.hpp src/ui/components/options/numberoption.hpp src/ui/components/options/submenuoption.hpp src/ui/components/options/toggleoption.hpp src/scripts/scripting/uitick.cpp src/scripts/scripting/uitick.hpp src/rage/util/exec_as_script.hpp src/rage/classes/net_game_event.hpp src/rage/types.hpp src/rage/classes/scr_vector.hpp src/hooking/hooking_helpers/detour.cpp src/hooking/hooking_helpers/vmt.cpp src/misc/common.cpp src/fiber/manager.cpp src/fiber/manager.hpp src/fiber/script.cpp src/fiber/script.hpp src/fiber/pool.cpp src/fiber/pool.hpp src/natives/crossmap.hpp src/natives/invoker.cpp src/natives/invoker.hpp src/natives/natives.hpp src/ui/components/options/togglenumberoption.hpp src/logger/stacktrace.cpp src/logger/stacktrace.hpp src/scripts/game/ui_disable_control.cpp src/scripts/game/ui_disable_control.hpp src/ui/components/options/togglelistoption.hpp src/memory/scanner/all.hpp src/memory/scanner/handle.hpp src/memory/scanner/module.cpp src/memory/scanner/module.hpp src/memory/scanner/pattern.cpp src/memory/scanner/pattern.hpp src/memory/scanner/range.cpp src/memory/scanner/range.hpp src/memory/scanner/sig_batch_scan.cpp src/memory/scanner/sig_batch_scan.hpp src/memory/scanner/fwdec.hpp src/rage/util/get.hpp src/rage/classes/net_connection_mgr.hpp src/rage/classes/net_connection_peer.hpp src/rage/util/breakup_kick.hpp src/ui/translation/translation_manager.cpp src/ui/translation/translation_manager.hpp src/player_mgr/player.cpp src/player_mgr/player.hpp src/player_mgr/manager.cpp src/player_mgr/manager.hpp src/hooking/hooked_fn_impl/player_join_leave/network_player_mgr.cpp src/hooking/hooked_fn_impl/player_join_leave/assign_physical_idx.cpp src/ui/components/keyboard.hpp src/scripts/scripting/job_queue.cpp src/scripts/scripting/job_queue.hpp src/scripts/game/loops.cpp src/scripts/game/loops.hpp src/looped_game/self_globals.cpp src/looped_game/self_globals.hpp src/looped_game/self_features.cpp src/looped_game/self_features.hpp src/misc/settings.hpp src/misc/settings.cpp src/rage/util/teleport.cpp src/rage/util/teleport.hpp src/rage/util/blip.cpp src/rage/util/blip.hpp src/rage/util/streaming.cpp src/rage/util/streaming.hpp src/ui/tabs/main_tab.cpp src/ui/tabs/main_tab.hpp src/ui/tabs/selected_player.cpp src/ui/tabs/selected_player.hpp src/ui/tabs/settings.cpp src/ui/tabs/settings.hpp src/ui/tabs/misc.cpp src/ui/tabs/misc.hpp src/ui/tabs/world.cpp src/ui/tabs/world.hpp src/ui/tabs/self.cpp src/ui/tabs/self.hpp src/ui/tabs/debug.cpp src/ui/tabs/debug.hpp src/ui/tabs/tab_includes.hpp src/rage/util/entity.cpp src/rage/util/entity.hpp src/ui/tabs/weapon.cpp src/ui/tabs/weapon.hpp src/looped_game/weapon.cpp src/looped_game/weapon.hpp src/rage/script/global.cpp src/rage/script/global.hpp src/rage/script/local.cpp src/rage/script/local.hpp src/rage/util/session_switcher.cpp src/rage/util/session_switcher.hpp src/rage/global_idx.hpp src/ui/tabs/network.cpp src/ui/tabs/network.hpp src/ui/tabs/all_players.cpp src/ui/tabs/all_players.hpp src/looped_game/transition_state.cpp src/looped_game/transition_state.hpp src/misc/hotkey_manager.cpp src/misc/hotkey_manager.hpp src/ui/components/keyboard.cpp src/hooking/native_hooking.cpp src/hooking/native_hooking.hpp src/hooking/hooking_helpers/script.cpp src/hooking/hooking_helpers/script.hpp src/rage/util/find_script_thread.hpp src/misc/json.cpp src/misc/json.hpp src/misc/thread_pool.cpp src/misc/thread_pool.hpp src/rage/data/data_loader.cpp src/rage/data/data_loader.hpp src/rage/util/fipack_reader.cpp src/rage/util/fipack_reader.hpp src/rage/classes/fidevice.cpp src/rage/classes/fidevice.hpp src/rage/data/data_types.cpp src/rage/data/data_types.hpp src/rage/classes/datBitBuffer.hpp src/pch.hpp src/ui/tabs/vehicle.cpp src/ui/tabs/vehicle.hpp src/rage/util/vehicle.cpp src/rage/util/vehicle.hpp src/lua/manager.cpp src/lua/manager.hpp src/lua/script.cpp src/lua/script.hpp  src/memory/scanner/cached_batch.cpp src/memory/scanner/cached_batch.hpp src/lua/script_manifest.cpp src/lua/script_manifest.hpp src/lua/script_info.hpp)

include(../vendor/sol2.cmake)
include(../vendor/gta_classes.cmake)
include(../vendor/robin_hood.cmake)
include(../vendor/fmt.cmake)
include(../vendor/spdlog.cmake)
include(../vendor/minhook.cmake)
include(../vendor/freetype.cmake)
include(../vendor/rapidjson.cmake)
include(../vendor/jsoncpp.cmake)
include(../vendor/magic_enum.cmake)
include(../vendor/pugixml.cmake)
include(../vendor/stb.cmake)
include(../vendor/xorstr.cmake)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)

target_include_directories(${PROJECT_NAME} PRIVATE ../vendor/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE ../vendor/binary_bakery)
target_include_directories(${PROJECT_NAME} PRIVATE ../vendor/discord-rpc/include)

target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)