//
// Created by X-ray on 05/09/2023.
//

#include "renderer.hpp"
#include <d3d12.h>
#include <dxgi.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx12.h>
#include <imgui/imgui_impl_win32.h>
#include "imgui_input_queue.hpp"
#include "../hooking/hooking.hpp"
#include "../hooking/wndproc.hpp"
#include "../memory/pointers.hpp"
#include "../ui/menu_renderer.hpp"
#include <vector>

namespace base::menu::render {
  Renderer::Renderer() : d3d12_context_(*memory::kPOINTERS->swap_chain_, *memory::kPOINTERS->command_queue_) {
    d3d12_context_.Initialize();
    SetResolution(d3d12_context_.GetSwapChainDesc().BufferDesc.Width, d3d12_context_.GetSwapChainDesc().BufferDesc.Height);
    kRENDERER = this;

    InitImGui();

    init_imgui_ = true;

    // Queue input events on message pump thread, process them on render thread before ImGui::NewFrame()
    // This avoids blocking WndProc which runs on the game's message pump thread
    wndproc_handler_id_ = hooking::kWNDPROC->AddWndProcHandler([](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
      ImGuiInputQueue::Get().QueueEvent(hwnd, msg, wparam, lparam);
    });
  }

  Renderer::~Renderer() {
    init_imgui_ = false;

    ImGuiInputQueue::Get().Clear();

    if (d3d12_context_.IsInitialized()) {
      d3d12_context_.WaitForLastFrame();

      d3d12_context_.WaitForAllFrames();
    }

    if (ImGui::GetCurrentContext()) {
      font_mgr_inst_.reset();
      ImGui_ImplDX12_InvalidateDeviceObjects();

      ImGui_ImplDX12_Shutdown();
      ImGui_ImplWin32_Shutdown();

      ImGui::DestroyContext();
    }

    kRENDERER = nullptr;

    if (wndproc_handler_id_ != 0) {
      hooking::kWNDPROC->RemoveWndProcHandler(wndproc_handler_id_);
      wndproc_handler_id_ = 0;
    }

    d3d12_context_.Cleanup();
  }

  void Renderer::InitImGui() {
    auto game_hwnd = win32::GetGameHwnd();
    if (game_hwnd.error()) {
      LOG_CRITICAL("Failed to get window handle: {}", game_hwnd);
      throw std::runtime_error("Failed to get window handle");
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(game_hwnd.value());

    ImGui_ImplDX12_InitInfo init_info = {};
    init_info.Device = d3d12_context_.GetDevice();
    init_info.CommandQueue = d3d12_context_.GetCommandQueue();
    init_info.NumFramesInFlight = d3d12_context_.GetSwapChainDesc().BufferCount;
    init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
    init_info.SrvDescriptorHeap = d3d12_context_.GetDescriptorHeap();
    init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) {
      kRENDERER->d3d12_context_.GetDescriptorHeapAllocator().Alloc(out_cpu_handle, out_gpu_handle);
    };
    init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, const D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, const D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) {
      kRENDERER->d3d12_context_.GetDescriptorHeapAllocator().Free(cpu_handle, gpu_handle);
    };
    ImGui_ImplDX12_Init(&init_info);

    LOG_DEBUG("Initializing font manager.");
    font_mgr_inst_ = std::make_unique<imfont::Manager>();
  }

  // Static state for mouse input dispatch
  static ImVec2 last_mouse_pos = {};
  static bool last_left_clicked = false;
  static bool last_right_clicked = false;
  static float last_wheel = 0.0f;

  void DispatchMouseInputToListeners(const std::vector<util::input::MouseInputListener*>& listeners) {
    const ImGuiIO& io = ImGui::GetIO();

    // Dispatch left click
    if (io.MouseClicked[0] && !last_left_clicked) {
      for (auto listener : listeners) {
        listener->OnMouseLeftClick();
      }
    }
    last_left_clicked = io.MouseClicked[0];

    // Dispatch right click
    if (io.MouseClicked[1] && !last_right_clicked) {
      for (auto listener : listeners) {
        listener->OnMouseRightClick();
      }
    }
    last_right_clicked = io.MouseClicked[1];

    // Dispatch wheel
    if (io.MouseWheel != 0.0f && io.MouseWheel != last_wheel) {
      const float wheel_delta = io.MouseWheel > 0 ? 1.0f : -1.0f;
      for (auto listener : listeners) {
        listener->OnMouseWheel(wheel_delta);
      }
    }
    last_wheel = io.MouseWheel;
  }

  HRESULT Renderer::Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    if (!globals::kRUNNING || !kRENDERER || !kRENDERER->init_imgui_ || kRENDERER->d3d12_context_.IsResizing()) {
      return hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&Present)>(hooking::Hooks::swapchain_present_index, swap_chain, sync_interval, flags);
    }

    try {
      const std::uint64_t now = common::util::time::GetTimeStamp();
      kRENDERER->SetDeltaTime(now - kRENDERER->GetLastTime());
      kRENDERER->SetLastTime(now);

      kRENDERER->d3d12_context_.NewFrame();

      imfont::kMANAGER->PushFont("roboto");

      // Update ImGui IO flags based on cursor requests (reference counting)
      const bool show_cursor = kRENDERER->cursor_show_requests_ > 0;
      ImGui::GetIO().MouseDrawCursor = show_cursor;
      ImGui::GetIO().WantCaptureMouse = show_cursor;

      // Dispatch mouse events to listeners
      DispatchMouseInputToListeners(kRENDERER->mouse_input_listeners_);

      kRENDERER->GetDrawQueueBuffer()->RenderFrame();
      imfont::kMANAGER->PopFont();

      kRENDERER->d3d12_context_.EndFrame();
    } catch (const std::exception& e) {
      LOG_ERROR("Exception in Present: {}", e.what());
    }

    return hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&Present)>(hooking::Hooks::swapchain_present_index, swap_chain, sync_interval, flags);
  }

  HRESULT Renderer::ResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
    if (globals::kRUNNING && kRENDERER) {
      kRENDERER->SetResolution(width, height);

      if (kRENDERER->init_imgui_)
        kRENDERER->d3d12_context_.PreResize();
      const auto res = hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
      if (kRENDERER->init_imgui_)
        kRENDERER->d3d12_context_.PostResize();

      return res;
    }

    return hooking::kMANAGER->swap_chain_hook_.CallOriginal<decltype(&ResizeBuffers)>(hooking::Hooks::swapchain_resizebuffers_index, swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
  }

  void Renderer::RequestShowCursor() {
    cursor_show_requests_++;
  }

  void Renderer::RequestHideCursor() {
    if (cursor_show_requests_ > 0) {
      cursor_show_requests_--;
    }
  }

  void Renderer::RegisterMouseInputListener(util::input::MouseInputListener* listener) {
    if (!listener) return;
    mouse_input_listeners_.push_back(listener);
  }

  void Renderer::UnregisterMouseInputListener(util::input::MouseInputListener* listener) {
    if (!listener) return;
    auto it = std::find(mouse_input_listeners_.begin(), mouse_input_listeners_.end(), listener);
    if (it != mouse_input_listeners_.end()) {
      mouse_input_listeners_.erase(it);
    }
  }
}
