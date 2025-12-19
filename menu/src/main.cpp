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
#include "render/thread.hpp"
#include "ui/localization/manager.hpp"
#include "ui/notification/manager.hpp"
#include "util/thread_pool.hpp"

std::atomic<bool> base::menu::globals::kRUNNING = true;

namespace base::menu {
  namespace {
    std::unique_ptr<util::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::WndProc> wndproc_inst;
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
    std::unique_ptr<ui::localization::Manager> localization_manager_inst;
    std::unique_ptr<discord::RichPresence> discord_rich_presence_inst;
    std::unique_ptr<render::Renderer> render_inst;
    std::unique_ptr<render::Thread> render_thread_manager_inst;
    std::unique_ptr<std::thread> render_thread_inst;
    std::unique_ptr<ui::notification::Manager> notification_manager_inst;
  }
}

class LifeTimeHelper {
public:
  enum Action {
    Init,
    Shutdown
  };

  using cb_t = std::function<void(Action)>;

public:
  LifeTimeHelper() = default;

  ~LifeTimeHelper() {
    LOG_INFO("[SHUTDOWN] LifeTimeHelper");
    for (auto it = cb_vec_.rbegin(); it != cb_vec_.rend(); ++it)
      (*it)(Shutdown);
  }

  void RunInit() const {
    LOG_INFO("[INIT] LifeTimeHelper");
    for (auto& cb : cb_vec_)
      cb(Init);
  }

  void AddCallback(const cb_t& cb) {
    cb_vec_.push_back(cb);
  }

private:
  std::vector<cb_t> cb_vec_;
};

#define MANAGER_PTR_LIFETIME(lifetime_helper_inst, init_name, manager_var, ...) lifetime_helper_inst->AddCallback([](LifeTimeHelper::Action action) { \
  if (action == LifeTimeHelper::Init) { \
    manager_var = std::make_unique<std::remove_reference_t<decltype(*manager_var)>>(__VA_ARGS__); \
    LOG_INFO("[INIT] {}", xorstr_(init_name)); \
  } \
  else { \
    manager_var.reset(); \
    LOG_INFO("[SHUTDOWN] {}", xorstr_(init_name)); \
  } \
})

void ThreadPoolLifetime(LifeTimeHelper* life_time_helper) {
  life_time_helper->AddCallback([&](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      base::menu::thread_pool_inst = std::make_unique<std::remove_reference_t<decltype(*base::menu::thread_pool_inst)>>(std::thread::hardware_concurrency() / 2);
      base::menu::util::kTHREAD_POOL = base::menu::thread_pool_inst.get();
      LOG_INFO("[INIT] {}", "ThreadPool");
    } else {
      base::menu::util::kTHREAD_POOL = nullptr;
      base::menu::thread_pool_inst.reset();
      LOG_INFO("[SHUTDOWN] {}", "ThreadPool");
    }
  });
}

void RenderThreadLifeTime(LifeTimeHelper* lifetime_helper) {
  lifetime_helper->AddCallback([](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      LOG_INFO("[INIT] Renderer");
      base::menu::render_inst = std::make_unique<base::menu::render::Renderer>();
      base::menu::render_thread_manager_inst = std::make_unique<base::menu::render::Thread>();

      base::menu::render::kTHREAD->AddRenderCallback(0, [](base::menu::render::DrawQueueBuffer* buffer) {
        buffer->AddCommand(base::menu::render::Text({0.5, 0.5}, ImColor(255, 0, 0), base::ui::localization::kMANAGER->Localize("text/hello_world"), 0.02F, false, true, true));
      });

      LOG_INFO("[INIT] Render thread");
      base::menu::render_thread_inst = std::make_unique<std::thread>(base::menu::render::Thread::RenderMain);
    } else {
      // First, make sure it's actually waiting, then unblock it.
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      base::menu::render::kRENDERER->GetDrawQueueBuffer()->UnblockRenderThread();

      LOG_DEBUG("[SHUTDOWN] Stopping render thread...");
      if (base::menu::render_thread_inst->joinable())
        base::menu::render_thread_inst->join();
      LOG_INFO("[SHUTDOWN] Render thread stopped.");

      base::menu::render_thread_manager_inst.reset();
      base::menu::render_inst.reset();
    }
  });
}

LRESULT UnloadKeyWatcher(HWND, const UINT msg, const WPARAM wparam, LPARAM) {
  if (msg == WM_KEYDOWN && wparam == VK_END) {
    LOG_INFO("Unloading key was pressed...");
    base::menu::globals::kRUNNING = false;
  }

  return 0;
}

int base::menu::menu_main() {
  auto lifetime_helper = std::make_unique<LifeTimeHelper>();

  ThreadPoolLifetime(lifetime_helper.get());
  MANAGER_PTR_LIFETIME(lifetime_helper, "Pointers", pointers_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "WndProc", wndproc_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "HookingManager", hooking_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "LocalizationManager", localization_manager_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "DiscordRichPresence", discord_rich_presence_inst);
  RenderThreadLifeTime(lifetime_helper.get());
  MANAGER_PTR_LIFETIME(lifetime_helper, "NotificationManager", notification_manager_inst);

  lifetime_helper->RunInit();

  hooking::kWNDPROC->AddWndProcHandler(UnloadKeyWatcher);

  hooking_inst->Enable();

  LOG_INFO("Loaded");
  NOTIFY_INFO("Menu Loaded", "Press END to unload the menu.");
  while (globals::kRUNNING) {
    discord::kRICH_PRESENCE->Tick();
    std::this_thread::yield();
  }
  LOG_INFO("Unloading...");

  hooking_inst->Disable();

  lifetime_helper.reset();

  return 0;
}

#undef MANAGER_PTR_LIFETIME
