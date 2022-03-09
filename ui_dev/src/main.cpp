//
// Created by X-ray on 3/9/2022.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "d3d11/d3d11.hpp"
#include "wndproc.hpp"
#include "ui/ui.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), NULL, NULL, NULL, NULL, "ui_dev", NULL };
  ::RegisterClassEx(&wc);

  HWND main_window = ::CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

  if (!D3D11::CreateDeviceD3D(main_window)) {
    D3D11::CleanupDeviceD3D();
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 1;
  }

  ::ShowWindow(main_window, SW_SHOWDEFAULT);
  ::UpdateWindow(main_window);

  MSG msg;
  bool show_demo_window;
  while (!GetAsyncKeyState(VK_ESCAPE)) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT) {
      break;
    }

    if (!ui::IsInitialized())
      ui::Initialize();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&show_demo_window);
    ui::Tick();

    ImGui::Render();
    const float clear_color_with_alpha[4] = { 0, 0, 0, 255 };
    D3D11::GetDeviceContext()->ClearRenderTargetView(D3D11::GetRenderTargetView(), clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    D3D11::GetSwapChain()->Present(1, 0);
  }
  ui::Shutdown();

  D3D11::CleanupDeviceD3D();
  ::DestroyWindow(main_window);
  ::UnregisterClass(wc.lpszClassName, wc.hInstance);
  return 0;
}