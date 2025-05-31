//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <chrono>
#include <memory>
#include <thread>
#include <fmt/args.h>
#include "discord/rich_presence.hpp"
#include "hooking/hooking.hpp"
#include "hooking/wndproc.hpp"
#include "memory/pointers.hpp"
#include "render/renderer.hpp"
#include "render/render_thread.hpp"
#include "scripts/script_manager.hpp"
#include "ui/localization/manager.hpp"
#include "ui/notification/manager.hpp"
#include "util/startup_shutdown_handler.hpp"
#include "util/thread_pool.hpp"

std::atomic<bool> base::menu::globals::kRUNNING = true;

namespace base::menu {
  namespace {
    std::unique_ptr<util::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::WndProc> wndproc_inst;
    std::unique_ptr<scripts::ScriptManager> script_manager_inst;
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
    std::unique_ptr<ui::localization::Manager> localization_manager_inst;
    std::unique_ptr<discord::RichPresence> discord_rich_presence_inst;
    std::unique_ptr<render::Renderer> render_inst;
    std::unique_ptr<ui::notification::Manager> notification_manager_inst;

    void SetupStartupShutdownSequence(util::StartupShutdownHandler* handler) {
      RegisterThreadPoolStartupShutdown(thread_pool_inst, handler);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "WndProc", wndproc_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "ScriptManager", script_manager_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "Pointers", pointers_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "HookingManager", hooking_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "LocalizationManager", localization_manager_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "DiscordRichPresence", discord_rich_presence_inst);
      render::RendererLifeTime(render_inst, handler);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "NotificationManager", notification_manager_inst);
    }
  }
}

LRESULT UnloadKeyWatcher(HWND, UINT msg, WPARAM wparam, LPARAM) {
  if (msg == WM_KEYDOWN && wparam == VK_END) {
    LOG_INFO("Unloading key was pressed...");
    base::menu::globals::kRUNNING = false;
  }

  return 0;
}

int base::menu::menu_main() {
  auto startup_shutdown_handler = std::make_unique<util::StartupShutdownHandler>();
  SetupStartupShutdownSequence(startup_shutdown_handler.get());

  startup_shutdown_handler->RunInit();

  hooking_inst->Enable();

  const auto unload_key_watcher_id = hooking::kWNDPROC->AddWndProcHandler(UnloadKeyWatcher);

  scripts::kSCRIPTMANAGER->InitScripts(scripts::BaseScript::Type::MainScriptThread);

  LOG_INFO("Loaded");
  while (globals::kRUNNING) {
    scripts::kSCRIPTMANAGER->TickScripts(scripts::BaseScript::Type::MainScriptThread);
    std::this_thread::yield();
  }
  LOG_INFO("Unloading...");

  hooking::kWNDPROC->RemoveWndProcHandler(unload_key_watcher_id);

  hooking_inst->Disable();

  startup_shutdown_handler.reset();

  return 0;
}

#undef MANAGER_PTR_LIFETIME
