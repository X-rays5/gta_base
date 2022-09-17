//
// Created by X-ray on 3/6/2022.
//

#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "logger/logger.hpp"
#include "misc/common.hpp"
#include "hooking/hooking.hpp"
#include "d3d/renderer.hpp"
#include "memory/pointers.hpp"
#include "scriptmanager/scriptmanager.hpp"
#include "scripts/scripting/discord.hpp"
#include "scripts/scripting/uitick.hpp"
#include "scripts/scripting/job_queue.hpp"
#include "scripts/render/uidraw.hpp"
#include "scripts/scripting/keyboard.hpp"
#include "scripts/game/ui_disable_phone.hpp"
#include "scripts/game/loops.hpp"
#include "rpc/discord.hpp"
#include "ui/manager.hpp"
#include "fiber/manager.hpp"
#include "fiber/pool.hpp"
#include "player_mgr/manager.hpp"
#include "ui/components/keyboard.hpp"

std::atomic<bool> gta_base::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  auto ptr_inst = std::make_unique<memory::Pointers>();
  LOG_INFO("Pointers initialized");

  LOG_INFO("Waiting for the game to load...");
  while(*memory::kPOINTERS->game_state_ != eGameState::Playing) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  LOG_INFO("Game loaded");

  MH_Initialize();
  auto hook_inst = std::make_unique<Hooking>();
  LOG_INFO("Hooking initialized");

  auto script_manager_inst = std::make_unique<ScriptManager>();
  LOG_INFO("Script Manager initialized");

  // render scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiDraw>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::Keyboard>());
  // scripting scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::Discord>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiTick>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::JobQueue>());
  // game scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<fiber::Manager>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UIDisablePhone>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::Loops>());
  LOG_INFO("kSCRIPT_MANAGER: added scripts now managing {} scripts", kSCRIPT_MANAGER->Count());

  auto fiber_inst = std::make_shared<fiber::Pool>(12);
  LOG_INFO("Created fiber pool");

  auto renderer_inst = std::make_unique<d3d::Renderer>(common::GetHwnd(globals::target_window_class, globals::target_window_name));
  LOG_INFO("Renderer initialized");

  auto ui_manager_inst = std::make_unique<ui::Manager>();
  LOG_INFO("UI Manager initialized");

  auto keyboard_manager_inst = std::make_unique<ui::components::keyboard::Manager>();
  LOG_INFO("Keyboard Manager initialized");

  auto discord_inst = std::make_unique<rpc::Discord>();
  LOG_INFO("Discord initialized");

  auto player_mgr_inst = std::make_unique<player_mgr::Manager>();
  LOG_INFO("Player Manager initialized");

  kHOOKING->Enable();
  LOG_INFO("Hooks enabled");

  auto scripting_thread = std::thread([]{
    while(globals::running) {
      kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kScripting);
      std::this_thread::yield();
    }
  });
  LOG_INFO("Scripting thread started");

  LOG_INFO("Initialized");
  while (globals::running) {
    if (common::IsKeyDown(VK_DELETE))
      globals::running = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  LOG_INFO("Unloading...");

  if (scripting_thread.joinable())
    scripting_thread.join();
  LOG_INFO("Scripting thread stopped");

  kHOOKING->Disable();
  LOG_INFO("Hooks disabled");

  player_mgr_inst.reset();
  LOG_INFO("Player Manager unloaded");

  discord_inst.reset();
  LOG_INFO("Discord shutdown");

  keyboard_manager_inst.reset();
  LOG_INFO("Keyboard Manager unloaded");

  ui_manager_inst.reset();
  LOG_INFO("UI Manager shutdown");

  renderer_inst.reset();
  LOG_INFO("Renderer shutdown");

  fiber_inst.reset();
  LOG_INFO("Fiber pool shutdown");

  script_manager_inst.reset();
  LOG_INFO("Script Manager shutdown");

  hook_inst.reset();
  MH_Uninitialize();
  LOG_INFO("Hooking shutdown");

  ptr_inst.reset();
  LOG_INFO("Pointers shutdown");
}