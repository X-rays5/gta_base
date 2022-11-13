//
// Created by X-ray on 07/31/22.
//

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#define BAKERY_PROVIDE_VECTOR
#include <binary_bakery_decoder.h>
#include "renderer.hpp"
#include "../hooking/hooking.hpp"
#include "../ui/fonts/fa.hpp"
#include "../ui/fonts/roboto_regular.hpp"
#include "../ui/fonts/roboto_bold.hpp"
#include "../ui/fonts/IconsFontAwesome6.h"
#include "../scriptmanager/scriptmanager.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gta_base::d3d {
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

      kRENDERER = this;
    }

    Renderer::~Renderer() {
      kRENDERER = nullptr;

      ImGui_ImplWin32_Shutdown();
      ImGui_ImplDX11_Shutdown();
      ImGui::DestroyContext();
    }

    ImFont* AddFont(const std::filesystem::path& path) {
      ImFontConfig cfg;
      cfg.FontDataOwnedByAtlas = false;
      return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.string().c_str(), 24, &cfg);
    }

    void MergeFa() {
      static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
      ImFontConfig icons_config;
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      icons_config.FontDataOwnedByAtlas = false;
      icons_config.GlyphOffset.y = 5;

      static const auto font_awesome = bb::decode_to_vector<std::uint8_t>(bb::get_payload("fa-solid-900.ttf"));
      ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)font_awesome.data(), font_awesome.size(), 24, &icons_config, icons_ranges);
    }

    void Renderer::InitD3D() {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplWin32_Init(hwnd_);
      ImGui_ImplDX11_Init(device_.Get(), device_context_.Get());

      ImFontConfig cfg;
      cfg.FontDataOwnedByAtlas = false;
      roboto_ = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(roboto_regular_compressed_data, roboto_regular_compressed_size, 24, &cfg);
      MergeFa();
      roboto_bold_ = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(roboto_bold_compressed_data, roboto_bold_compressed_size, 24, &cfg);
      MergeFa();
      if (!ImGui::GetIO().Fonts->IsBuilt())
        ImGui::GetIO().Fonts->Build();

      last_time_ = common::GetEpoch();
    }

    void Renderer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    }

    HRESULT Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
      if (globals::running) {
        std::uint64_t now = common::GetEpoch();
        kRENDERER->SetDeltaTime(now - kRENDERER->GetLastTime());
        kRENDERER->SetLastTime(now);

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
      if (globals::running) {
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