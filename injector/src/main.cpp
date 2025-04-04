//
// Created by X-ray on 10/02/2025.
//


#include <base-common/vfs.hpp>
#include <base-common/logging/logger.hpp>
#include <imgui/imgui.h>
#include "window.hpp"
#include "settings.hpp"

std::atomic<bool> kRUNNING = true;

std::unique_ptr<base::common::logging::Manager> kLOGGER;

constexpr auto kWINDOW_WIDTH = 850;
constexpr auto kWINDOW_HEIGHT = 510;
base::injector::Settings kSETTINGS;

void AtExit() {
  kRUNNING = false;

  if (const auto res = SaveSettings(kSETTINGS); !res) {
    LOG_ERROR("Failed to save settings: {}", res.error());
  }
  kLOGGER->Shutdown();
}

std::int32_t APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  using namespace base;
  using namespace base::injector;

  common::vfs::SetWorkingDir(BASE_SUBCOMPONENT);
  kLOGGER = std::make_unique<common::logging::Manager>();

#ifdef NDEBUG
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

  at_quick_exit(AtExit);
  atexit(AtExit);

  if (auto res = LoadSettings(); res) {
    kSETTINGS = res.value();
  } else {
    LOG_ERROR("Failed to load settings: {}", res.error());
    return 1;
  }

  const Window window(kWINDOW_WIDTH, kWINDOW_HEIGHT);

  while (kRUNNING) {
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

  return 0;
}
