//
// Created by X-ray on 07/31/22.
//

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#define BAKERY_PROVIDE_VECTOR
#include <binary_bakery_decoder.h>
#include "renderer.hpp"
#include "../hooking/hooking.hpp"
#include "../ui/fonts/roboto_mono.hpp"
#include "../ui/fonts/fa.h"
#include "../ui/fonts/IconsFontAwesome6.h"
#include "../misc/globals.hpp"
#include "../scriptmanager/scriptmanager.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gta_base {
  namespace d3d {
    Renderer::Renderer(HWND hwnd) {
      swap_chain_ = *memory::kPOINTERS->swap_chain_;
      hwnd_ = hwnd;

      void* device{};
      if (FAILED(swap_chain_->GetDevice(__uuidof(ID3D11Device), &device))) {
        LOG_FATAL("Failed to get device from swap chain");
      }
      device_.Attach(static_cast<ID3D11Device*>(device));
      device_->GetImmediateContext(device_context_.GetAddressOf());

      InitD3D();

      texture_manager_ = std::make_unique<TextureManager>();
      kRENDERER = this;
    }

    Renderer::~Renderer() {
      texture_manager_.reset();
      kRENDERER = nullptr;

      ImGui_ImplWin32_Shutdown();
      ImGui_ImplDX11_Shutdown();
      ImGui::DestroyContext();
    }

    void Renderer::InitD3D() {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplWin32_Init(hwnd_);
      ImGui_ImplDX11_Init(device_.Get(), device_context_.Get());

      ImFontConfig cfg;
      cfg.FontDataOwnedByAtlas = false;
      roboto_ = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(roboto_mono_compressed_data, roboto_mono_compressed_size, 24, &cfg);

      static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
      ImFontConfig icons_config;
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      icons_config.FontDataOwnedByAtlas = false;

      auto font_awesome = bb::decode_to_vector<std::uint8_t>(bb::get_payload("fa-solid-900.ttf"));
      ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)font_awesome.data(), font_awesome.size(), 24, &icons_config, icons_ranges);
      if (!ImGui::GetIO().Fonts->IsBuilt())
        ImGui::GetIO().Fonts->Build();
    }

    void Renderer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }

    HRESULT Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
      if (common::globals::running) {
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(kRENDERER->GetFont());
        kSCRIPT_MANAGER->Tick(scriptmanager::ScriptType::kRenderer);
        ImGui::PopFont();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
      }

      return kHOOKING->swap_chain_hook_.GetOriginal<decltype(&Hooks::Present)>(Hooks::swapchain_present_index)(swap_chain, sync_interval, flags);
    }

    HRESULT Renderer::SwapChainResizeBuffer(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags) {
      if (common::globals::running) {
        ImGui_ImplDX11_InvalidateDeviceObjects();

        auto res = kHOOKING->swap_chain_hook_.GetOriginal<decltype(&Hooks::ResizeBuffers)>(Hooks::swapchain_resizebuffers_index)(swap_chain, buffer_count, width, height, new_format, swapchain_flags);
        if (SUCCEEDED(res))
          ImGui_ImplDX11_CreateDeviceObjects();

        return res;
      } else {
        return kHOOKING->swap_chain_hook_.GetOriginal<decltype(&Hooks::ResizeBuffers)>(Hooks::swapchain_resizebuffers_index)(swap_chain, buffer_count, width, height, new_format, swapchain_flags);
      }
    }
  }
}