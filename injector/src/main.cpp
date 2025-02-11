//
// Created by X-ray on 10/02/2025.
//

#include <cstdint>
#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <Windows.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

std::int32_t APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        MessageBoxA(nullptr, "Failed to initialize SDL3", "Error", MB_ICONERROR);
        return 1;
    }

    constexpr Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
    SDL_Window* window = SDL_CreateWindow("GTA base injector", 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool done = false;
    int x_res{};
    int y_res{};
    SDL_GetWindowSize(window, &x_res, &y_res);

    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(x_res, y_res), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);

        {
            ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.
            ImGui::End();
        }

        ImGui::Render();
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}