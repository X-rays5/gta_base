//
// Created by X-ray on 06/04/2025.
//
#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include "test_globals.hpp"

base::common::concurrency::Spinlock kRENDER_LOCK;
std::atomic<bool> kRUNNING = true;

void SDLMainThread() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    exit(-1);
  }

  constexpr Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
  SDL_Window* window = SDL_CreateWindow("imguitest", 20, 20, window_flags);
  if (window == nullptr) {
    exit(-1);
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    exit(-1);
  }

  SDL_SetRenderVSync(renderer, 1);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  while (kRUNNING) {
    base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // don't care about events
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("imguitest")) {
      ImGui::Text("Hello, world!");
      ImGui::End();
    }

    ImGui::Render();
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    std::this_thread::yield();
  }

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  std::jthread thread(SDLMainThread);
  // Let SDL and ImGui init
  std::this_thread::sleep_for(std::chrono::seconds(1));

  const auto ret_val = RUN_ALL_TESTS();
  kRUNNING = false;

  return ret_val;
}
