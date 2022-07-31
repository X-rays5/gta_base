//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_HOOKING_HPP
#define GTABASE_HOOKING_HPP
#include <memory>
#include <unordered_map>
#include <MinHook.h>
#include <d3d11.h>
#include "vmt.hpp"

namespace gta_base {
  struct Hooks {
    static constexpr auto swapchain_num_funcs = 18;
    static constexpr auto swapchain_present_index = 8;
    static constexpr auto swapchain_resizebuffers_index = 13;
    static HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);

  };

  class Hooking {
  public:
    hooking::VmtHook swap_chain_hook_;

  public:
    Hooking();
    ~Hooking();

    MH_STATUS AddHook(const std::string& name, LPVOID src, LPVOID dst, LPVOID* og);
    MH_STATUS RemoveHook(const std::string& name);
  private:
    std::unordered_map<std::string, LPVOID> hooks;
  };
  inline std::unique_ptr<Hooking> kHOOKING;
}
#endif //GTABASE_HOOKING_HPP
