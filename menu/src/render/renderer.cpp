//
// Created by X-ray on 05/09/2023.
//

#include "renderer.hpp"
#include <d3d11.h>
#include <dxgi.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include "../hooking/hooking.hpp"
#include <base-common/util/time.hpp>
#include "../hooking/wndproc.hpp"
#include "../memory/pointers.hpp"
#include "../scripts/script_manager.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace base::menu::render {
  namespace {
    void InitImGui(ID3D11Device* device, ID3D11DeviceContext* device_context) {
      LOG_DEBUG("ImGui init");
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplWin32_Init(win32::GetGameHwnd().value());
      ImGui_ImplDX11_Init(device, device_context);
    }

    void ShutdownImGui() {
      LOG_DEBUG("ImGui shutdown");
      ImGui_ImplWin32_Shutdown();
      ImGui_ImplDX11_Shutdown();
      ImGui::DestroyContext();
    }
  }

  Renderer::Renderer() {
    swap_chain_ = *memory::kPOINTERS->swap_chain_;
    LOG_CRITICAL_CONDITIONAL(!swap_chain_.GetAddressOf(), "Failed to find valid swapchain ptr.");

    void* device{};
    if (FAILED(swap_chain_->GetDevice(__uuidof(ID3D11Device), &device))) {
      LOG_CRITICAL("Failed to get device from swap chain");
    }

    device_.Attach(static_cast<ID3D11Device*>(device));
    device_->GetImmediateContext(device_ctx_.GetAddressOf());

    InitImGui(device_.Get(), device_ctx_.Get());
    init_imgui_ = true;

    DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
    if (!SUCCEEDED(swap_chain_->GetDesc(&swap_chain_desc))) {
      LOG_CRITICAL("Failed to get swap chain description");
      throw std::runtime_error("Failed to get swap chain description");
    }
    SetResolution(swap_chain_desc.BufferDesc.Width, swap_chain_desc.BufferDesc.Height);

    LOG_DEBUG("Initializing font manager.");
    font_mgr_inst_ = std::make_unique<imfont::Manager>();

    wndproc_handler_id_ = hooking::kWNDPROC->AddWndProcHandler(this);

    render_thread_ = std::make_unique<RenderThread>();

    kRENDERER = this;
  }

  Renderer::~Renderer() {
    kRENDERER = nullptr;

    render_thread_.reset();

    hooking::kWNDPROC->RemoveWndProcHandler(wndproc_handler_id_);

    font_mgr_inst_.reset();

    try {
      ShutdownImGui();
    } catch (const std::exception& e) {
      LOG_CRITICAL("Failed to shutdown ImGui: {}", e.what());
    } catch (...) {
      LOG_CRITICAL("Failed to shutdown ImGui due to an unknown error");
    }
  }

  void Renderer::OnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (globals::kRUNNING && init_imgui_) {
      ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }
  }

  HRESULT Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    if (globals::kRUNNING && kRENDERER && kRENDERER->init_imgui_) {
      const std::uint64_t now = common::util::time::GetTimeStamp();
      kRENDERER->SetDeltaTime(now - kRENDERER->GetLastTime());
      kRENDERER->SetLastTime(now);
      
      ImGui_ImplWin32_NewFrame();
      ImGui_ImplDX11_NewFrame();
      ImGui::NewFrame();

      imfont::kMANAGER->PushFont("roboto");
      kRENDERER->GetDrawQueueBuffer()->RenderFrame();
      scripts::kSCRIPTMANAGER->TickScripts(scripts::BaseScript::Type::GameRenderThread);
      imfont::kMANAGER->PopFont();

      ImGui::Render();
      ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    return hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&Present)>(hooking::Hooks::swapchain_present_index, swap_chain, sync_interval, flags);
  }

  HRESULT Renderer::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    if (globals::kRUNNING && kRENDERER && kRENDERER->init_imgui_) {
      kRENDERER->SetResolution(width, height);
      ImGui_ImplDX11_InvalidateDeviceObjects();

      const auto res = hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
      if (SUCCEEDED(res))
        ImGui_ImplDX11_CreateDeviceObjects();

      return res;
    }

    return hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }
}
