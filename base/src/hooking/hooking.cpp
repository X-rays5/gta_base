//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"
#include "../d3d/renderer.hpp"
#include "../scriptmanager/scriptmanager.hpp"
#include "../natives/invoker.hpp"

namespace gta_base {
  Hooking::Hooking() :
  swap_chain_hook_(*memory::kPOINTERS->swap_chain_),
  run_script_threads_hook_("RunScriptThreads", memory::kPOINTERS->RunScriptThreads, &Hooks::RunScriptThreads),
  network_player_mgr_init_hook_("NetworkPlayerMgrInit", memory::kPOINTERS->NetworkPlayerMgrInit, &Hooks::NetworkPlayerMgrInit),
  network_player_mgr_shutdown_hook_("NetworkPlayerMgrShutdown", memory::kPOINTERS->NetworkPlayerMgrShutdown, &Hooks::NetworkPlayerMgrShutdown),
  assign_physical_index_hook_("AssignPhysicalIndex", memory::kPOINTERS->AssignPhysicalIdx, &Hooks::assign_physical_index)
  {
    swap_chain_hook_.Hook("Present", Hooks::swapchain_present_index, &Hooks::Present);
    swap_chain_hook_.Hook("ResizeBuffers", Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers);

    kHOOKING = this;
  }

  Hooking::~Hooking() {
    swap_chain_hook_.Unhook(Hooks::swapchain_present_index);
    swap_chain_hook_.Unhook(Hooks::swapchain_resizebuffers_index);

    kHOOKING = nullptr;
  }

  void Hooking::Enable() {
    hooking::HookWndProc();
    swap_chain_hook_.Enable(Hooks::swapchain_present_index);
    swap_chain_hook_.Enable(Hooks::swapchain_resizebuffers_index);

    run_script_threads_hook_.Enable();
    network_player_mgr_init_hook_.Enable();
    network_player_mgr_shutdown_hook_.Enable();
    assign_physical_index_hook_.Enable();
  }

  void Hooking::Disable() {
    swap_chain_hook_.Disable(Hooks::swapchain_resizebuffers_index);
    swap_chain_hook_.Disable(Hooks::swapchain_present_index);

    assign_physical_index_hook_.Disable();
    network_player_mgr_shutdown_hook_.Disable();
    network_player_mgr_init_hook_.Disable();
    run_script_threads_hook_.Disable();

    hooking::UnhookWndProc();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    return d3d::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    return d3d::Renderer::SwapChainResizeBuffer(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }

  bool Hooks::RunScriptThreads(std::uint32_t ops_to_execute) {
    try {
      if (globals::running) {
        kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kGame);
      }
    } catch(...) {
      LOG_WARN("Exception was thrown in RunScriptThreads");
    }

    return kHOOKING->run_script_threads_hook_.GetOriginal<decltype(&Hooks::RunScriptThreads)>()(ops_to_execute);
  }
}