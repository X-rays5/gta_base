//
// Created by X-ray on 02/12/2023.
//

#ifndef HOOKING_HPP
#define HOOKING_HPP
#include <dxgi.h>
#include "helpers/detour.hpp"
#include "helpers/vmt.hpp"

namespace base::menu::hooking {
  struct Hooks {
    static constexpr auto swapchain_present_index = 8;
    static constexpr auto swapchain_resizebuffers_index = 13;
    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);
    static bool RunScriptThreads(int ops_to_execute);
  };

  class Manager {
  public:
    VmtHook swap_chain_hook_;
    DetourHook run_script_threads_hook_;

  public:
    Manager();
    ~Manager();

    void Enable();
    void Disable();
  };

  inline Manager* kMANAGER{};
}
#endif //HOOKING_HPP
