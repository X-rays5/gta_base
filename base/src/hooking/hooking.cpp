//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"
#include "../logger/logger.hpp"
#include "../d3d/renderer.hpp"
#include "../misc/common.hpp"
#include "../scriptmanager/scriptmanager.hpp"

namespace gta_base {
  static LPVOID* OutputDebugStringA_og{};
  static LPVOID* OutputDebugStringA_target{};

  Hooking::Hooking() :
  swap_chain_hook_(*memory::kPOINTERS->swap_chain_),
  run_script_threads_hook_("RunScriptThreads", memory::kPOINTERS->RunScriptThreads, &Hooks::RunScriptThreads)
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

    MH_CreateHookApiEx(L"KERNELBASE", "OutputDebugStringA", Hooks::OutputDebugStringA, OutputDebugStringA_og, OutputDebugStringA_target);

    run_script_threads_hook_.Enable();
    MH_EnableHook(OutputDebugStringA_target);
  }

  void Hooking::Disable() {
    swap_chain_hook_.Disable(Hooks::swapchain_resizebuffers_index);
    swap_chain_hook_.Disable(Hooks::swapchain_present_index);

    MH_DisableHook(OutputDebugStringA_target);
    run_script_threads_hook_.Disable();

    MH_RemoveHook(OutputDebugStringA_target);

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
      if (common::globals::running) {
        kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kGame);
      }
    } catch(...) {
      LOG_WARN("Exception was thrown in RunScriptThreads");
    }

    return kHOOKING->run_script_threads_hook_.GetOriginal<decltype(&Hooks::RunScriptThreads)>()(ops_to_execute);
  }

  void Hooks::OutputDebugStringA(LPCSTR lpOutputString) {
    LOG_DEBUG("OutputDebugStringA: {}", lpOutputString);
  }
}