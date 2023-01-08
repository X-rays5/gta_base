//
// Created by X-ray on 3/6/2022.
//

#include "hooking/hooking.hpp"
#include "d3d/renderer.hpp"
#include "memory/pointers.hpp"
#include "scriptmanager/scriptmanager.hpp"
#include "scripts/scripting/uitick.hpp"
#include "scripts/scripting/job_queue.hpp"
#include "scripts/render/uidraw.hpp"
#include "scripts/game/ui_disable_control.hpp"
#include "scripts/game/loops.hpp"
#include "rpc/discord.hpp"
#include "ui/manager.hpp"
#include "fiber/manager.hpp"
#include "fiber/pool.hpp"
#include "player_mgr/manager.hpp"
#include "ui/components/keyboard.hpp"
#include "misc/thread_pool.hpp"
#include "rage/data/data_loader.hpp"
#include "lua/manager.hpp"
#include "settings/option_state.hpp"
#include "settings/profile.hpp"

std::atomic<bool> gta_base::globals::running = true;
static bool waited_for_game_load = false;
rage::data::Data gta_base::globals::gta_data = {};

void BaseMain() {
  using namespace gta_base;

  auto thread_pool_inst = std::make_unique<misc::ThreadPool>();
  LOG_INFO("Created thread pool with {} threads", thread_pool_inst->GetThreadCount());

  auto ptr_inst = std::make_unique<memory::Pointers>();
  LOG_INFO("Pointers initialized");

  MH_Initialize();
  auto hook_inst = std::make_unique<Hooking>();
  LOG_INFO("Hooking initialized");

  auto script_manager_inst = std::make_unique<ScriptManager>();
  LOG_INFO("Script Manager initialized");

  // render scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiDraw>());
  // main thread scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiTick>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::JobQueue>());
  // game scripts
  kSCRIPT_MANAGER->AddScript(std::make_shared<fiber::Manager>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UIDisablePhone>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::Loops>());
  LOG_INFO("kSCRIPT_MANAGER: added scripts now managing {} scripts", kSCRIPT_MANAGER->Count());

  auto fiber_inst = std::make_unique<fiber::Pool>(12);
  LOG_INFO("Created fiber pool");

  auto renderer_inst = std::make_unique<d3d::Renderer>(common::GetHwnd(globals::target_window_class, globals::target_window_name));
  LOG_INFO("Renderer initialized");

  if (*memory::kPOINTERS->game_state_ != eGameState::Playing) {
    waited_for_game_load = true;
    LOG_INFO("Waiting for the game to load...");
  }
  while (*memory::kPOINTERS->game_state_ != eGameState::Playing) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  if (waited_for_game_load) {
    LOG_INFO("Game loaded");
  }

  auto gta_data_loader = std::make_unique<rage::data::Loader>();
  fiber::kPOOL->AddJob([&] {
    auto gta_data = gta_data_loader->Load();
    if (!gta_data.has_value()) {
      LOG_CRITICAL("Failed to load gta data");
      return;
    }
    globals::gta_data = std::move(gta_data.value());
    gta_data_loader.reset();
  });

  auto ui_manager_inst = std::make_unique<ui::Manager>();
  LOG_INFO("UI Manager initialized");

  auto keyboard_manager_inst = std::make_unique<ui::keyboard::Manager>();
  LOG_INFO("Keyboard Manager initialized");

  auto discord_inst = std::make_unique<rpc::Discord>();
  LOG_INFO("Discord initialized");

  auto player_mgr_inst = std::make_unique<player_mgr::Manager>();
  LOG_INFO("Player Manager initialized");

  kHOOKING->Enable();
  LOG_INFO("Hooks enabled");

  settings::option_state::Load(settings::option_state::GetSavePath());
  LOG_INFO("Settings loaded");

  while (globals::gta_data.IsEmpty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  auto discord_thread = std::thread([] {
    while (globals::running) {
      rpc::kDISCORD->SetLargeImage("gta-logo");
      if (common::IsSessionStarted()) {
        auto player_mgr = rage::GetNetworkPlayerMgr();
        rpc::kDISCORD->SetStatus(fmt::format("Playing online ({} of {})", player_mgr->m_player_count, player_mgr->m_player_limit));
      } else {
        rpc::kDISCORD->SetStatus("Playing offline");
      }

      rpc::kDISCORD->Tick();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  });
  LOG_INFO("Started Discord thread");

  auto scripting_thread = std::thread([] {
    while (globals::running) {
      kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kScripting);
      if (lua::kMANAGER) {
        lua::kMANAGER->RunScriptTick();
      }
      std::this_thread::yield();
    }
  });
  LOG_INFO("Scripting thread started");

  auto lua_manager_inst = std::make_unique<lua::Manager>();
  LOG_INFO("Lua Manager initialized");

  LOG_INFO("Initialized");
  while (globals::running) {
    if (common::IsKeyDown(VK_DELETE))
      globals::running = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  LOG_INFO("Unloading...");

  lua_manager_inst.reset();
  LOG_INFO("Lua Manager unloaded");

  if (scripting_thread.joinable())
    scripting_thread.join();
  LOG_INFO("Scripting thread stopped");

  if (discord_thread.joinable())
    discord_thread.join();
  LOG_INFO("Discord thread stopped");

  if (kSETTINGS.menu.save_option_state_on_exit) {
    settings::option_state::Save(settings::option_state::GetSavePath());
    LOG_INFO("Settings saved");
  }

  kHOOKING->Disable();
  LOG_INFO("Hooks disabled");

  player_mgr_inst.reset();
  LOG_INFO("Player Manager shutdown");

  discord_inst.reset();
  LOG_INFO("Discord shutdown");

  keyboard_manager_inst.reset();
  LOG_INFO("Keyboard Manager shutdown");

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

  thread_pool_inst.reset();
  LOG_INFO("Thread pool shutdown");
}