//
// Created by X-ray on 05/09/2023.
//

#include "renderer.hpp"
#include <dxgi.h>
#include <imgui/imgui.h>
#include <d3d11.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include "../memory/pointers.hpp"
#include "../hooking/hooking.hpp"

namespace base::render {
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
    LOG_FATAL_CONDITIONAL(!swap_chain_.GetAddressOf(), "Failed to find valid swapchain ptr.");

    void* device{};
    if (FAILED(swap_chain_->GetDevice(__uuidof(ID3D11Device), &device))) {
      LOG_FATAL("Failed to get device from swap chain");
    }

    device_.Attach(static_cast<ID3D11Device*>(device));
    device_->GetImmediateContext(device_ctx_.GetAddressOf());

    InitImGui(device_.Get(), device_ctx_.Get());

    LOG_DEBUG("Initializing font manager.");
    font_mgr_inst_ = std::make_unique<font::Manager>();

    kRENDERER = this;
  }

  Renderer::~Renderer() {
    kRENDERER = nullptr;

    font_mgr_inst_.reset();

    ShutdownImGui();
  }

  HRESULT Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    if (globals::kRUNNING) {
      ImGui_ImplWin32_NewFrame();
      ImGui_ImplDX11_NewFrame();
      ImGui::NewFrame();

      font::kMANAGER->PushFont("default");
      kRENDERER->GetDrawQueueBuffer()->RenderFrame();
      font::kMANAGER->PopFont();

      ImGui::Render();
      ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    return hooking::kMANAGER->swap_chain_hook_.GetOriginal<decltype(&Present)>(hooking::Hooks::swapchain_present_index, swap_chain, sync_interval, flags);
  }

  HRESULT Renderer::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    if (globals::kRUNNING) {
      ImGui_ImplDX11_InvalidateDeviceObjects();

      auto res = hooking::kMANAGER->swap_chain_hook_.GetOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
      if (SUCCEEDED(res))
        ImGui_ImplDX11_CreateDeviceObjects();

      return res;
    }

    return hooking::kMANAGER->swap_chain_hook_.GetOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }
}
