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
#include "scripts/game/discord.hpp"
#include "scripts/game/uitick.hpp"
#include "scripts/render/uidraw.hpp"
#include "rpc/discord.hpp"
#include "ui/manager.hpp"

std::atomic<bool> gta_base::common::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  auto logger_inst = std::make_unique<Logger>();
  LOG_INFO << "Logging initialized";

  auto og_terminate_handler = std::get_terminate();
  std::set_terminate([]{
    try {
      std::rethrow_exception(std::current_exception());
    } catch(std::runtime_error& e) {
      LOG_FATAL << "Uncaught exception occurred: " << e.what();
    } catch(...) {
      LOG_FATAL << "Unknown exception occurred";
    }

    abort();
  });
  LOG_INFO << "Terminate handler set";

  auto ptr_inst = std::make_unique<memory::Pointers>();
  LOG_INFO << "Pointers initialized";

  LOG_INFO << "Waiting for the game to load...";
  while(*memory::kPOINTERS->game_state_ != rage::GameState::kPlaying) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  LOG_INFO << "Game loaded";

  MH_Initialize();
  auto hook_inst = std::make_unique<Hooking>();
  LOG_INFO << "Hooking initialized";

  auto script_manager_inst = std::make_unique<ScriptManager>();
  LOG_INFO << "Script Manager initialized";

  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::Discord>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiTick>());
  kSCRIPT_MANAGER->AddScript(std::make_shared<scripts::UiDraw>());
  LOG_INFO << "Scripts added";

  auto renderer_inst = std::make_unique<d3d::Renderer>(common::GetHwnd(common::globals::target_window_class, common::globals::target_window_name));
  LOG_INFO << "Renderer initialized";

  auto ui_manager_inst = std::make_unique<ui::Manager>();
  LOG_INFO << "UI Manager initialized";

  auto discord_inst = std::make_unique<rpc::Discord>();
  LOG_INFO << "Discord initialized";

  auto scripting_thread = std::thread([]{
    while(common::globals::running)
      kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kGame); // TODO: actually make this a game thread
      kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kScripting);
  });
  LOG_INFO << "Scripting thread started";

  kHOOKING->Enable();
  LOG_INFO << "Hooks enabled";

  LOG_INFO << "Initialized";
  while (common::globals::running) {
    if (common::KeyState(VK_DELETE))
      common::globals::running = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  LOG_INFO << "Unloading...";

  kHOOKING->Disable();
  LOG_INFO << "Hooks disabled";

  discord_inst.reset();
  LOG_INFO << "Discord shutdown";

  ui_manager_inst.reset();
  LOG_INFO << "UI Manager shutdown";

  renderer_inst.reset();
  LOG_INFO << "Renderer shutdown";

  if (scripting_thread.joinable())
    scripting_thread.join();
  LOG_INFO << "Scripting thread stopped";

  script_manager_inst.reset();
  LOG_INFO << "Script Manager shutdown";

  hook_inst.reset();
  MH_Uninitialize();
  LOG_INFO << "Hooking shutdown";

  ptr_inst.reset();
  LOG_INFO << "Pointers shutdown";

  std::set_terminate(og_terminate_handler);
  LOG_INFO << "Terminate handler restored";

  LOG_INFO << "Shutting logging down...";
  logger_inst.reset();
}
