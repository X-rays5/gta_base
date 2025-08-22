//
// Created by X-ray on 02/12/2023.
//

#include "hooking.hpp"
#include "../memory/pointers.hpp"
#include "../render/renderer.hpp"
#include "../logging/plh_logger.hpp"

namespace base::menu::hooking {
  Manager::Manager() :
    swap_chain_hook_("swap_chain", *memory::kPOINTERS->swap_chain_, {
                       {Hooks::swapchain_present_index, &Hooks::Present},
                       {Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers}
                     }) {
    const auto logger = std::make_shared<logging::PLH::Logger>();
    PLH::Log::registerLogger(logger);

    kMANAGER = this;
  }

  void Manager::Enable() {
    swap_chain_hook_.EnableAll();
  }

  void Manager::Disable() {
    swap_chain_hook_.DisableAll();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, const UINT sync_interval, const UINT flags) {
    return render::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, const UINT buffer_count, const UINT width, const UINT height, const DXGI_FORMAT new_format, const UINT swap_chain_flags) {
    return render::Renderer::ResizeBuffers(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }
}
