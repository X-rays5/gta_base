//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"
#include "../logger/logger.hpp"
#include "../memory/pointers.hpp"
#include "../d3d/renderer.hpp"

namespace gta_base {
  Hooking::Hooking() : swap_chain_hook_(*memory::kPOINTERS->swap_chain_, Hooks::swapchain_num_funcs) {
    MH_Initialize();
    hooking::HookWndProc();

    swap_chain_hook_.Hook(Hooks::swapchain_present_index, &d3d::Renderer::Present);
    swap_chain_hook_.Hook(Hooks::swapchain_resizebuffers_index, &d3d::Renderer::SwapChainResizeBuffer);

    swap_chain_hook_.Enable();
  }

  Hooking::~Hooking() {
    swap_chain_hook_.Disable();
    swap_chain_hook_.Unhook(Hooks::swapchain_present_index);
    swap_chain_hook_.Unhook(Hooks::swapchain_resizebuffers_index);

    MH_DisableHook(MH_ALL_HOOKS);
    for (auto &hook : hooks) {
      MH_RemoveHook(hook.second);
    }
    MH_Uninitialize();
    hooking::UnhookWndProc();
  }

  MH_STATUS Hooking::AddHook(const std::string& name, LPVOID src, LPVOID dst, LPVOID* og) {
    MH_CreateHook(src, dst, og);
    MH_STATUS result = MH_EnableHook(src);
    hooks[name] = src;

    if (result == MH_OK) {
#ifdef NDEBUG
      LOG_INFO("[{}] hooked", name);
#else
      LOG_DEBUG("[{}] hooked src -> {}, dst -> {}, og -> {}", name, (void*)src, (void*)dst, (void*)og);
#endif
    }

    return result;
  }

  MH_STATUS Hooking::RemoveHook(const std::string& name) {
    auto hook_src = hooks[name];
    hooks.erase(name);

    MH_DisableHook(hook_src);
    return MH_RemoveHook(hook_src);
  }
}