//
// Created by X-ray on 07/31/22.
//

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "renderer.hpp"
#include "../hooking/hooking.hpp"
#include "../memory/pointers.hpp"
#include "../logger/logger.hpp"
#include "../ui/fonts/roboto_mono.hpp"
#include "../common/globals.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gta_base {
  namespace d3d {
    Renderer::Renderer(HWND hwnd) {
      swap_chain_ = *memory::kPOINTERS->swap_chain_;
      hwnd_ = hwnd;

      void* device{};
      if (FAILED(swap_chain_->GetDevice(__uuidof(ID3D11Device), &device))) {
        // TODO: log
      }
      device_.Attach(static_cast<ID3D11Device*>(device));
      device_->GetImmediateContext(device_context_.GetAddressOf());

      InitD3D();
    }

    Renderer::~Renderer() {
      ImGui_ImplWin32_Shutdown();
      ImGui_ImplDX11_Shutdown();
      ImGui::DestroyContext();
    }

    void Renderer::InitD3D() {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplWin32_Init(hwnd_);
      ImGui_ImplDX11_Init(device_.Get(), device_context_.Get());

      roboto_ = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(roboto_mono_compressed_data, roboto_mono_compressed_size, 36);
      if (!ImGui::GetIO().Fonts->IsBuilt())
        ImGui::GetIO().Fonts->Build();
    }

    void Renderer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }

    void Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
      if (common::globals::running) {
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(kRENDERER->GetFont());
        ImGui::Begin("Test");
        ImGui::Text("gamer moment confirmed");
        ImGui::End();
        // TODO: call d3d scripts here
        LOG_DEBUG("Present");
        ImGui::PopFont();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        kHOOKING->swap_chain_hook_.GetOriginal<decltype(&Present)>(Hooks::swapchain_present_index)(swap_chain, sync_interval, flags);
      }
    }

    HRESULT Renderer::SwapChainResizeBuffer(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags) {
      if (common::globals::running) {
        ImGui_ImplDX11_InvalidateDeviceObjects();

        auto res = kHOOKING->swap_chain_hook_.GetOriginal<decltype(&SwapChainResizeBuffer)>(Hooks::swapchain_resizebuffers_index)(swap_chain, buffer_count, width, height, new_format, swapchain_flags);
        if (SUCCEEDED(res))
          ImGui_ImplDX11_CreateDeviceObjects();

        return res;
      }

      return NULL;
    }
  }
}