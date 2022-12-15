//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"
#include "../d3d/renderer.hpp"
#include "../scriptmanager/scriptmanager.hpp"
#include "../natives/invoker.hpp"
#include "../rage/util/exec_as_script.hpp"

namespace gta_base {
  Hooking::Hooking() :
  swap_chain_hook_(*memory::kPOINTERS->swap_chain_),
  network_player_mgr_init_hook_("NetworkPlayerMgrInit", memory::kPOINTERS->NetworkPlayerMgrInit, &Hooks::NetworkPlayerMgrInit),
  network_player_mgr_shutdown_hook_("NetworkPlayerMgrShutdown", memory::kPOINTERS->NetworkPlayerMgrShutdown, &Hooks::NetworkPlayerMgrShutdown),
  assign_physical_index_hook_("AssignPhysicalIndex", memory::kPOINTERS->AssignPhysicalIdx, &Hooks::AssignPlayerPhysicalIdx),
  run_script_threads_hook_("RunScriptThreads", memory::kPOINTERS->RunScriptThreads, &Hooks::RunScriptThreads),
  gta_thread_start_hook_("GtaThreadStart", memory::kPOINTERS->GtaThreadStart, &Hooks::GtaThreadStart),
  gta_thread_kill_hook_("GtaThreadKill", memory::kPOINTERS->GtaThreadKill, &Hooks::GtaThreadKill)
  {
    swap_chain_hook_.Hook("Present", Hooks::swapchain_present_index, &Hooks::Present);
    swap_chain_hook_.Hook("ResizeBuffers", Hooks::swapchain_resizebuffers_index, &Hooks::ResizeBuffers);

    kHOOKING = this;
    native_hooking_inst_ = std::make_unique<hooking::NativeHooking>();
  }

  Hooking::~Hooking() {
    swap_chain_hook_.Unhook(Hooks::swapchain_present_index);
    swap_chain_hook_.Unhook(Hooks::swapchain_resizebuffers_index);

    native_hooking_inst_.reset();
    kHOOKING = nullptr;
  }

  void Hooking::Enable() {
    hooking::HookWndProc();
    swap_chain_hook_.Enable(Hooks::swapchain_present_index);
    swap_chain_hook_.Enable(Hooks::swapchain_resizebuffers_index);

    network_player_mgr_init_hook_.Enable();
    network_player_mgr_shutdown_hook_.Enable();
    assign_physical_index_hook_.Enable();
    run_script_threads_hook_.Enable();
    gta_thread_start_hook_.Enable();
    gta_thread_kill_hook_.Enable();
  }

  void Hooking::Disable() {
    swap_chain_hook_.Disable(Hooks::swapchain_resizebuffers_index);
    swap_chain_hook_.Disable(Hooks::swapchain_present_index);

    gta_thread_kill_hook_.Disable();
    gta_thread_start_hook_.Disable();
    run_script_threads_hook_.Disable();
    assign_physical_index_hook_.Disable();
    network_player_mgr_shutdown_hook_.Disable();
    network_player_mgr_init_hook_.Disable();

    hooking::UnhookWndProc();
  }

  HRESULT Hooks::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    return d3d::Renderer::Present(swap_chain, sync_interval, flags);
  }

  HRESULT Hooks::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    return d3d::Renderer::SwapChainResizeBuffer(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }

  inline void ScriptThreadTick() {
    rage::kINVOKER.CacheHandlers();

    if (globals::running) {
      rage::util::ExecuteAsScript(RAGE_JOAAT("main_persistent"), [&] {
        static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);

        kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kGame);
      });
    }
  }

  bool Hooks::RunScriptThreads(std::uint32_t ops_to_execute) {
    __try {
      ScriptThreadTick();

      return kHOOKING->run_script_threads_hook_.GetOriginal<decltype(&Hooks::RunScriptThreads)>()(ops_to_execute);
    } __except (LOG_ERROR("Exception in RunScriptThreads: {}", GetExceptionInformation()->ExceptionRecord->ExceptionCode), EXCEPTION_CONTINUE_EXECUTION) {
      return false;
    }
  }

  GtaThread* Hooks::GtaThreadStart(unsigned int** a1, unsigned int a2) {
    const auto res = kHOOKING->gta_thread_start_hook_.GetOriginal<decltype(&Hooks::GtaThreadStart)>()(a1, a2);
    if (!res)
      return res;

    if (!std::string(res->m_name).empty()) {
      LOG_DEBUG("Script thread '{}' was started", res->m_name);
    } else {
      LOG_DEBUG("Script thread '{}' was started", res->m_script_hash);
    }

    hooking::kNATIVE_HOOKING->ThreadStart(res);

    return res;
  }

  static std::string last_kill{};
  rage::eThreadState Hooks::GtaThreadKill(GtaThread* thread) {
    const auto res = kHOOKING->gta_thread_kill_hook_.GetOriginal<decltype(&GtaThreadKill)>()(thread);
    if (last_kill != thread->m_name)
      LOG_DEBUG("Script thread '{}' was killed", thread->m_name);
    last_kill = thread->m_name;

    hooking::kNATIVE_HOOKING->ThreadKill(thread);

    return res;
  }
}