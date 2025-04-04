//
// Created by X-ray on 10/02/2025.
//


#include <base-common/vfs.hpp>
#include <base-common/logging/logger.hpp>
#include <imgui/imgui.h>
#include "window.hpp"

std::unique_ptr<base::common::logging::Manager> kLOGGER;

constexpr auto kWINDOW_WIDTH = 850;
constexpr auto kWINDOW_HEIGHT = 510;

[[noreturn]] std::int32_t APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  base::common::vfs::SetWorkingDir(BASE_SUBCOMPONENT);
  kLOGGER = std::make_unique<base::common::logging::Manager>();

#ifdef NDEBUG
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

  at_quick_exit([]() {
    kLOGGER->Shutdown();
  });
  atexit([]() {
    kLOGGER->Shutdown();
  });

  const base::injector::Window window(kWINDOW_WIDTH, kWINDOW_HEIGHT);

  while (true) {
    window.HandleEvents();
    if (SDL_GetWindowFlags(window.GetWindow()) & SDL_WINDOW_MINIMIZED) {
      std::this_thread::yield();
      continue;
    }

    window.PreFrame();

    {
      if (ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Hello, world!");
        ImGui::End();
      }
    }

    window.PostFrame();
  }
}
