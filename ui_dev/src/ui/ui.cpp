//
// Created by X-ray on 3/9/2022.
//

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "../d3d11/D3D11.h"
#include "ui.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ui {
  bool initialized = false;
  bool IsInitialized() {
    return initialized;
  }

  void Initialize() {
    if (initialized) {
      return;
    }

    ImGui::CreateContext();
    ImGui_ImplDX11_Init(D3D11::getInstance().GetDevice(), D3D11::getInstance().GetDeviceContext());
    ImGui_ImplWin32_Init(FindWindowA("ui_dev", "ui_dev"));
  }

  void Shutdown() {
    if (!initialized) {
      return;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
  }

  inline void FrameStart() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

  }

  inline void FrameEnd() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }

  void Tick() {
    FrameStart();



    FrameEnd();
  }

  LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr) {
    if (ImGui::GetCurrentContext())
      ImGui_ImplWin32_WndProcHandler(window, message, parameter_uint_ptr, parameter_long_ptr);

    return 0;
  }
}