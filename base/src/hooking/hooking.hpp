//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_HOOKING_HPP
#define GTABASE_HOOKING_HPP
#include <memory>
#include <robin_hood.h>
#include <MinHook.h>
#include <d3d11.h>
#include "hooking_helpers/vmt.hpp"
#include "hooking_helpers/detour.hpp"
#include "../memory/pointers.hpp"

namespace gta_base {
  struct Hooks {
    static constexpr auto swapchain_num_funcs = 18;
    static constexpr auto swapchain_present_index = 8;
    static constexpr auto swapchain_resizebuffers_index = 13;
    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);
    static void NetworkPlayerMgrInit(CNetworkPlayerMgr* that, std::uint64_t a2, std::uint32_t a3, std::uint32_t a4[4]);
    static void NetworkPlayerMgrShutdown(CNetworkPlayerMgr* that);
    static void* assign_physical_index(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, uint8_t new_index);

    static bool RunScriptThreads(std::uint32_t ops_to_execute);
  };

  class Hooking {
  public:
    hooking::VmtHook swap_chain_hook_;
    hooking::DetourHook run_script_threads_hook_;
    hooking::DetourHook network_player_mgr_init_hook_;
    hooking::DetourHook network_player_mgr_shutdown_hook_;
    hooking::DetourHook assign_physical_index_hook_;

  public:
    Hooking();
    ~Hooking();

    void Enable();
    void Disable();
  };
  inline Hooking* kHOOKING{};
}
#endif //GTABASE_HOOKING_HPP
