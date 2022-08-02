//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"
#include "../logger/logger.hpp"
#include "../d3d/renderer.hpp"

namespace gta_base {
  Hooking::Hooking() :
  swap_chain_hook_(*memory::kPOINTERS->swap_chain_, Hooks::swapchain_num_funcs),
  send_minidump("SendMinidump", memory::kPOINTERS->send_minidump, &Hooks::SendMiniDump) {
    swap_chain_hook_.Hook(Hooks::swapchain_present_index, &Hooks::Present);
    swap_chain_hook_.Hook(Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers);

    kHOOKING = this;
  }

  Hooking::~Hooking() {
    swap_chain_hook_.Unhook(Hooks::swapchain_present_index);
    swap_chain_hook_.Unhook(Hooks::swapchain_resizebuffers_index);

    kHOOKING = nullptr;
  }

  void Hooking::Enable() {
    hooking::HookWndProc();
    swap_chain_hook_.Enable();

    send_minidump.Enable();

  }

  void Hooking::Disable() {
    swap_chain_hook_.Disable();

    send_minidump.Disable();
    hooking::UnhookWndProc();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    return d3d::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    return d3d::Renderer::SwapChainResizeBuffer(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }

  char Hooks::SendMiniDump() {
    LOG_FATAL << "Minidump is being sent";

    return kHOOKING->send_minidump.GetOriginal<decltype(&SendMiniDump)>()();
  }
}