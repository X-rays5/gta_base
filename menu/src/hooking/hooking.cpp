//
// Created by X-ray on 02/12/2023.
//

#include "hooking.hpp"
#include "../memory/pointers.hpp"
#include "../render/renderer.hpp"

namespace base::hooking {
  Manager::Manager() :
    swap_chain_hook_(*memory::kPOINTERS->swap_chain_) {
    swap_chain_hook_.Hook("Present", Hooks::swapchain_present_index, &Hooks::Present);
    swap_chain_hook_.Hook("ResizeBuffers", Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers);


    kMANAGER = this;
  }

  void Manager::Enable() {
    swap_chain_hook_.EnableAll();
  }

  void Manager::Disable() {
    swap_chain_hook_.DisableAll();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    return render::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    return render::Renderer::ResizeBuffers(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }
}
