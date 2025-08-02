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
#include "scripts/script_manager.hpp"
#include "ui/localization/manager.hpp"
#include "ui/menu_renderer.hpp"
#include "ui/components/execute_component.hpp"
#include "ui/components/label_component.hpp"
#include "ui/components/sub_link_component.hpp"
#include "util/startup_shutdown_handler.hpp"
#include "util/thread_pool.hpp"
#include "util/key_input/key_event_listener.hpp"

std::atomic_bool base::menu::globals::kRUNNING = true;

namespace base::menu {
  namespace {
    std::unique_ptr<scripts::ScriptManager> script_manager_inst;
    std::unique_ptr<util::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::WndProc> wndproc_inst;
    std::unique_ptr<util::KeyEventWatcher> keywatch_inst;
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
    std::unique_ptr<ui::localization::Manager> localization_manager_inst;
    std::unique_ptr<discord::RichPresence> discord_rich_presence_inst;
    std::unique_ptr<render::Renderer> render_inst;
    std::unique_ptr<ui::MenuRenderer> menu_renderer_inst;

    void SetupStartupShutdownSequence(util::StartupShutdownHandler* handler) {
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "ScriptManager", script_manager_inst);
      RegisterThreadPoolStartupShutdown(thread_pool_inst, handler);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "WndProc", wndproc_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "KeyEventWatcher", keywatch_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "Pointers", pointers_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "HookingManager", hooking_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "LocalizationManager", localization_manager_inst);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "DiscordRichPresence", discord_rich_presence_inst);
      render::RendererLifeTime(render_inst, handler);
      GTA_BASE_DEFAULT_START_DOWN_HANDLER(handler, "MenuRenderer", menu_renderer_inst);
    }
  }
}

class UnloadKeyWatcher final : public base::menu::hooking::WndProcEventListener {
public:
  ~UnloadKeyWatcher() override = default;

  void OnWndProc(HWND, const UINT msg, const WPARAM wparam, LPARAM) override {
    if (msg == WM_KEYDOWN && wparam == VK_END) {
      LOG_INFO("Unloading key was pressed...");
      base::menu::globals::kRUNNING = false;
    }
  }
};

DWORD base::menu::menu_main() {
  UnloadKeyWatcher unload_key_watcher;
  auto startup_shutdown_handler = std::make_unique<util::StartupShutdownHandler>();
  SetupStartupShutdownSequence(startup_shutdown_handler.get());

  startup_shutdown_handler->RunInit();

  hooking_inst->Enable();

  ui::Submenu home_submenu("Home", [](ui::Submenu* sub) {
    sub->AddComponent(ui::components::LabelComponent("label/home_submenu"));
    sub->AddComponent(ui::components::ExecuteComponent("Execute Script", [](ui::components::ExecuteComponent*) {
      LOG_INFO("Executing script from home submenu.");
      // Here you can add the logic to execute a script or perform an action.
    }));
    sub->AddComponent(ui::components::SubLinkComponent("non existing sub"));
  });
  ui::kMENU_RENDERER->AddSubmenu(ui::SubmenuIDs::kMAIN_MENU, std::move(home_submenu));

  const auto unload_key_watcher_id = hooking::kWNDPROC->AddWndProcHandler(&unload_key_watcher);

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
