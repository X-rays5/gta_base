//
// Created by X-ray on 05/09/2023.
//

#include "manager.hpp"
#include <dxgi.h>
#include <kiero.h>
#include <imgui.h>
#include <imgui_impl_dx10.h>
#include <d3d10.h>
#include <d3d11.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <magic_enum.hpp>

#define IMGUI_CALL_RENDER_BACKEND(fn, ...) render_api == kiero::RenderType::D3D11 ? ImGui_ImplDX11_##fn(__VA_ARGS__) : ImGui_ImplDX10_##fn(__VA_ARGS__)

namespace base::render {
  namespace {
    bool imgui_init = false;
    kiero::RenderType::Enum render_api;

    void InitImGui(IDXGISwapChain* swap_chain) {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplWin32_Init(win32::GetGameHwnd());

      void* device{};
      swap_chain->GetDevice(__uuidof(ID3D11Device), &device);

      if (render_api == kiero::RenderType::D3D10 || render_api == kiero::RenderType::D3D10_1) {
        ImGui_ImplDX10_Init(static_cast<ID3D10Device*>(device));
      } else if (render_api == kiero::RenderType::D3D11) {
        ID3D11DeviceContext* device_ctx{};
        static_cast<ID3D11Device*>(device)->GetImmediateContext(&device_ctx);
        ImGui_ImplDX11_Init(static_cast<ID3D11Device*>(device), device_ctx);
      } else {
        LOG_CRITICAL("Kiero has been init with and unsupported render api");
        abort(); //TODO: should implement something to abort loading and do a clean exit
      }
    }

    void ShutdownImGui() {
      ImGui_ImplWin32_Shutdown();

      if (render_api == kiero::RenderType::D3D10 || render_api == kiero::RenderType::D3D10_1) {
        ImGui_ImplDX10_Shutdown();
      } else if (render_api == kiero::RenderType::D3D11) {
        ImGui_ImplDX11_Shutdown();
      } else {
        LOG_CRITICAL("Unsupported render api was encountered when shutting down renderer");
      }

      ImGui::DestroyContext();
    }

    HRESULT Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
      if (globals::kRUNNING) {
        if (!imgui_init) {
          InitImGui(swap_chain);
          imgui_init = true;
        }

        ImGui_ImplWin32_NewFrame();
        IMGUI_CALL_RENDER_BACKEND(NewFrame);
        ImGui::NewFrame();

        for (auto&& cb : kMANAGER->GetRenderCallbacks()) {
          cb();
        }

        ImGui::Render();
        IMGUI_CALL_RENDER_BACKEND(RenderDrawData, ImGui::GetDrawData());
      }

      return reinterpret_cast<decltype(&Present)>(kMANAGER->GetPresentOg())(swap_chain, sync_interval, flags);
    }

    HRESULT ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
      if (globals::kRUNNING) {
        IMGUI_CALL_RENDER_BACKEND(InvalidateDeviceObjects);

        auto res = reinterpret_cast<decltype(&ResizeBuffers)>(kMANAGER->GetResizeBuffersOg())(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
        if (SUCCEEDED(res))
          IMGUI_CALL_RENDER_BACKEND(CreateDeviceObjects);

        return res;
      }

      return reinterpret_cast<decltype(&ResizeBuffers)>(kMANAGER->GetResizeBuffersOg())(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
    }
  }

  Manager::Manager() {
    if (auto status = kiero::init(kiero::RenderType::Auto); status != kiero::Status::Success) {
      LOG_CRITICAL("Kiero failed to init: {}", magic_enum::enum_name(status));
      abort(); //TODO: should implement something to abort loading and do a clean exit
    }
    render_api = kiero::getRenderType();

    kiero::bind(8, &present_og_, &Present);
    kiero::bind(13, &resize_buffers_og_, &ResizeBuffers);
  }

  Manager::~Manager() {
    kMANAGER = nullptr;

    ShutdownImGui();
    imgui_init = false;
    kiero::shutdown();
  }
}