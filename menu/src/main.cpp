//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <memory>
#include "util/thread_pool.hpp"
#include "render/renderer.hpp"
#include "memory/pointers.hpp"
#include "hooking/hooking.hpp"
#include "ui/localization/manager.hpp"
#include "memory/signature/pattern.hpp"
#include "discord/rich_presence.hpp"
#include <chrono>

std::atomic<bool> base::globals::kRUNNING = true;

namespace base {
  namespace {
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<util::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
    std::unique_ptr<ui::localization::Manager> localization_manager_inst;
    std::unique_ptr<discord::RichPresence> discord_rich_presence_inst;
    std::unique_ptr<render::Renderer> render_inst;
    std::unique_ptr<render::Thread> render_thread_manager_inst;
    std::unique_ptr<std::thread> render_thread_inst;
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
    LOG_INFO("[INIT] {}", init_name); \
  } \
  else { \
    manager_var.reset(); \
    LOG_INFO("[SHUTDOWN] {}", init_name); \
  } \
})

void ThreadPoolLifetime(LifeTimeHelper* life_time_helper) {
  life_time_helper->AddCallback([&](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      base::thread_pool_inst = std::make_unique<std::remove_reference_t<decltype(*base::thread_pool_inst)>>(std::thread::hardware_concurrency() / 2);
      base::util::kTHREAD_POOL = base::thread_pool_inst.get();
      LOG_INFO("[INIT] {}", "ThreadPool");
    } else {
      base::util::kTHREAD_POOL = nullptr;
      base::thread_pool_inst.reset();
      LOG_INFO("[SHUTDOWN] {}", "ThreadPool");
    }
  });
}

void RenderThreadLifeTime(LifeTimeHelper* lifetime_helper) {
  lifetime_helper->AddCallback([](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      LOG_INFO("[INIT] Renderer");
      base::render_inst = std::make_unique<base::render::Renderer>();
      base::render_thread_manager_inst = std::make_unique<base::render::Thread>();

      base::render::kTHREAD->AddRenderCallback([](base::render::DrawQueueBuffer* buffer) {
        buffer->AddCommand(base::render::Text({0.5, 0.5}, ImColor(255, 0, 0), base::ui::localization::kMANAGER->Localize("text/hello_world"), false, true, 0.02F));
      });

      LOG_INFO("[INIT] Render thread");
      base::render_thread_inst = std::make_unique<std::thread>(base::render::Thread::RenderMain);
    } else {
      // First make sure it's actually waiting then unblock it.
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      base::render::kRENDERER->GetDrawQueueBuffer()->UnblockRenderThread();

      LOG_DEBUG("[SHUTDOWN] Stopping render thread...");
      if (base::render_thread_inst->joinable())
        base::render_thread_inst->join();
      LOG_INFO("[SHUTDOWN] Render thread stopped.");

      base::render_thread_manager_inst.reset();
      base::render_inst.reset();
    }
  });
}

int base::menu_main() {
  auto lifetime_helper = std::make_unique<LifeTimeHelper>();

  ThreadPoolLifetime(lifetime_helper.get());
  MANAGER_PTR_LIFETIME(lifetime_helper, "Pointers", pointers_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "HookingManager", hooking_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "LocalizationManager", localization_manager_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "DiscordRichPresence", discord_rich_presence_inst);
  RenderThreadLifeTime(lifetime_helper.get());

  lifetime_helper->RunInit();

  hooking_inst->Enable();

  LOG_INFO("Loaded");
  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END)) {
      globals::kRUNNING = false;
      break;
    }

    discord::kRICH_PRESENCE->Tick();
    std::this_thread::yield();
  }
  LOG_INFO("Unloading...");

  hooking_inst->Disable();

  lifetime_helper.reset();

  return 0;
}

#undef MANAGER_PTR_LIFETIME
