//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <memory>
#pragma warning(push, 0)
#include <ThreadPool/ThreadPool.h>
#pragma warning(pop)
#include <MinHook.hpp>
#include "render/renderer.hpp"
#include "memory/pointers.hpp"
#include "hooking/hooking.hpp"

std::atomic<bool> base::globals::kRUNNING = true;

namespace base {
  namespace {
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<hmthrp::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
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

    void AddCallback(const cb_t& cb) { cb_vec_.push_back(cb); }

  private:
    std::vector<cb_t> cb_vec_;
};

#define MANAGER_PTR_LIFETIME(lifetime_helper_inst, init_name, manager_var) lifetime_helper_inst->AddCallback([](LifeTimeHelper::Action action) { \
  if (action == LifeTimeHelper::Init) { \
    manager_var = std::make_unique<std::remove_reference_t<decltype(*manager_var)>>(); \
    LOG_INFO("[INIT] {}", init_name); \
  } \
  else { \
    manager_var.reset(); \
    LOG_INFO("[SHUTDOWN] {}", init_name); \
  } \
})

int base::menu_main() {
  auto lifetime_helper = std::make_unique<LifeTimeHelper>();

  MANAGER_PTR_LIFETIME(lifetime_helper, "Pointers", pointers_inst);

  lifetime_helper->AddCallback([](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      MH_Initialize();
      LOG_INFO("[INIT] MinHook");
      hooking_inst = std::make_unique<hooking::Manager>();
      LOG_INFO("[INIT] HookingManager");
    } else {
      hooking_inst.reset();
      LOG_INFO("[SHUTDOWN] HookingManager");
      MH_Uninitialize();
      LOG_INFO("[SHUTDOWN] MinHook");
    }
  });

  lifetime_helper->AddCallback([](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      LOG_INFO("[INIT] Renderer");
      render_inst = std::make_unique<render::Renderer>();
      render_thread_manager_inst = std::make_unique<render::Thread>();

      render::kTHREAD->AddRenderCallback([](render::DrawQueueBuffer* buffer) {
        buffer->AddCommand(render::Text({0.5, 0.5}, ImColor(255, 0, 0), "Hello World!", false, true, 0.02f));
      });

      LOG_INFO("[INIT] Render thread");
      render_thread_inst = std::make_unique<std::thread>(render::Thread::RenderMain);
    } else {
      // First make sure it's actually waiting then unblock it.
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      render::kRENDERER->GetDrawQueueBuffer()->UnblockRenderThread();

      LOG_DEBUG("[SHUTDOWN] Stopping render thread...");
      if (render_thread_inst->joinable())
        render_thread_inst->join();
      LOG_INFO("[SHUTDOWN] Render thread stopped.");

      render_thread_manager_inst.reset();
      render_inst.reset();
    }
  });

  lifetime_helper->RunInit();

  hooking_inst->Enable();

  LOG_INFO("Loaded");
  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END)) {
      globals::kRUNNING = false;
      break;
    }
  }
  LOG_INFO("Unloading...");

  hooking_inst->Disable();

  lifetime_helper.reset();

  return 0;
}

#undef MANAGER_PTR_LIFETIME
