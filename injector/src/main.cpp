//
// Created by X-ray on 10/02/2025.
//

#include <future>
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logger.hpp>
#include <imgui/imgui.h>
#include <magic_enum/magic_enum.hpp>
#include "inject.hpp"
#include "settings.hpp"
#include "window.hpp"

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

  common::fs::vfs::SetWorkingDir(BASE_SUBCOMPONENT);
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

  auto dll_path = std::make_unique<char[]>(MAX_PATH);
  auto target_window_name = std::make_unique<char[]>(255);
  auto target_window_class = std::make_unique<char[]>(255);
  auto target_process_name = std::make_unique<char[]>(255);

  // init to initial values
  strncpy_s(dll_path.get(), MAX_PATH, kSETTINGS.dll_path.c_str(), _TRUNCATE);
  strncpy_s(target_window_name.get(), 255, kSETTINGS.target_window_name.c_str(), _TRUNCATE);
  strncpy_s(target_window_class.get(), 255, kSETTINGS.target_window_class.c_str(), _TRUNCATE);
  strncpy_s(target_process_name.get(), 255, kSETTINGS.target_process_name.c_str(), _TRUNCATE);

  HWND game_wnd = nullptr;

  while (kRUNNING) {
    window.HandleEvents();
    if (SDL_GetWindowFlags(window.GetWindow()) & SDL_WINDOW_MINIMIZED) {
      std::this_thread::yield();
      continue;
    }

    game_wnd = win32::GetHwnd(kSETTINGS.target_window_class, kSETTINGS.target_window_name).value_or(nullptr);

    window.PreFrame();

    {
      if (ImGui::Begin("Injector window", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::BeginTabBar("tabs")) {
          if (ImGui::BeginTabItem("Injector")) {
            ImGui::Text(!game_wnd ? "Game not running" : fmt::format("Game running: {}", win32::GetPIDFromHWND(game_wnd).value_or(0)).c_str());
            if (game_wnd) {
              if (ImGui::Button("Inject")) {
                auto _ = std::async(std::launch::async, [=]() {
                  if (const auto res = Inject(win32::GetPIDFromHWND(game_wnd).value_or(0), kSETTINGS.dll_path); res.error()) {
                    LOG_ERROR("Failed to inject: {}", res.error());
                    MessageBoxA(nullptr, res.error().GetResultMessage().c_str(), "Error", MB_OK | MB_ICONERROR);
                  } else {
                    LOG_INFO("Injected successfully");
                  }
                });
              }
            }

            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Settings")) {
            ImGui::Text("Settings");

            ImGui::InputText("DLL Path", dll_path.get(), MAX_PATH);
            ImGui::InputText("Target Window Name", target_window_name.get(), 255);
            ImGui::InputText("Target Window Class", target_window_class.get(), 255);
            ImGui::InputText("Target Process Name", target_process_name.get(), 255);

            kSETTINGS.dll_path = dll_path.get();
            kSETTINGS.target_window_name = target_window_name.get();
            kSETTINGS.target_window_class = target_window_class.get();
            kSETTINGS.target_process_name = target_process_name.get();

            if (ImGui::Button("Save")) {
              if (const auto res = SaveSettings(kSETTINGS); !res) {
                LOG_ERROR("Failed to save settings: {}", res.error());
              }
            }

            if (ImGui::Button("Open injector files dir")) {
              system(fmt::format("explorer.exe {}", std::filesystem::current_path()).c_str());
            }

            ImGui::EndTabItem();
          }

          ImGui::EndTabBar();
        }

        ImGui::End();
      }
    }

    window.PostFrame();
  }

  return 0;
}
