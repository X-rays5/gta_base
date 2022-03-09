//
// Created by X-ray on 3/9/2022.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "d3d11/D3D11.h"
#include "wndproc.hpp"
#include "ui/ui.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  WNDCLASSEXA wc = {sizeof(wc), CS_VREDRAW | CS_HREDRAW, WndProc, NULL, NULL, GetModuleHandleA(nullptr), nullptr, nullptr, (HBRUSH)COLOR_WINDOW, nullptr, "ui_dev", nullptr};
  if (!RegisterClassExA(&wc)) {
    MessageBoxA(nullptr, "RegisterClassExA failed", "Error", MB_OK | MB_ICONERROR);
    return 1;
  }

  HWND main_window = CreateWindowExA(NULL, "ui_dev", "ui_dev", WS_VISIBLE | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, wc.hInstance, nullptr);
  ShowCursor(0);

  D3D11::getInstance().initialize(1920, 1080, main_window, true);

  MSG msg;
  while (!GetAsyncKeyState(VK_ESCAPE)) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (!ui::IsInitialized())
      ui::Initialize();

    ui::Tick();

    D3D11::getInstance().clearRenderTargetViews();
    D3D11::getInstance().endScene();
  }
  ui::Shutdown();

  D3D11::getInstance().destroy();
  return 0;
}