//
// Created by X-ray on 02/12/2023.
//

#include "hooking.hpp"
#include "../game/script.hpp"
#include "../logging/plh_logger.hpp"
#include "../memory/pointers.hpp"
#include "../render/renderer.hpp"
#include "../script/script_manager.hpp"

namespace base::menu::hooking {
  Manager::Manager() :
    swap_chain_hook_("swap_chain", *memory::kPOINTERS->swap_chain_, {
                       {Hooks::swapchain_present_index, &Hooks::Present},
                       {Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers}
                     }),
    run_script_threads_hook_("run_script_threads", memory::kPOINTERS->run_script_threads_, &Hooks::RunScriptThreads)
  {
    const auto logger = std::make_shared<logging::PLH::Logger>();
    PLH::Log::registerLogger(logger);

    kMANAGER = this;
  }

  Manager::~Manager() {
    PLH::Log::registerLogger(nullptr);

    kMANAGER = nullptr;
  }

  void Manager::Enable() {
    swap_chain_hook_.EnableAll();
    run_script_threads_hook_.Enable();
  }

  void Manager::Disable() {
    run_script_threads_hook_.Disable();
    swap_chain_hook_.DisableAll();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, const UINT sync_interval, const UINT flags) {
    return render::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, const UINT buffer_count, const UINT width, const UINT height, const DXGI_FORMAT new_format, const UINT swap_chain_flags) {
    return render::Renderer::ResizeBuffers(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }

  bool Hooks::RunScriptThreads(const int ops_to_execute) {
    const auto retVal = kMANAGER->run_script_threads_hook_.CallOriginal<decltype(&RunScriptThreads)>(ops_to_execute);
    if (globals::kRUNNING) {
      script::kSCRIPT_MANAGER->TickScripts(script::ScriptBase::Type::GameScript);
      auto thread = game::FindScriptThread("freemode"_J);
      if (!thread)
        thread = game::FindScriptThread("main_persistent"_J);
      if (!thread)
        thread = game::FindScriptThread("startup"_J);

      if (thread) {
        game::RunAsScript(thread, [] {script::kSCRIPT_MANAGER->TickScripts(script::ScriptBase::Type::GameScript);});
      }
    }

    return retVal;
  }
}
