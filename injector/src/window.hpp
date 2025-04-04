//
// Created by X-ray on 04/04/2025.
//

#ifndef WINDOW_HPP_04125118
#define WINDOW_HPP_04125118
#include <base-common/result.hpp>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <imfont/imfont.hpp>

namespace base::injector {
  class Window {
  public:
    Window(int width, int height);
    ~Window();

    void HandleEvents() const;
    void PreFrame() const;
    void PostFrame() const;

    FORCE_INLINE SDL_Window* GetWindow() const {
      return window_;
    }

    FORCE_INLINE SDL_Renderer* GetRenderer() const {
      return renderer_;
    }

  private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::unique_ptr<imfont::Manager> font_manager_;
    int width_;
    int height_;

  private:
    [[nodiscard]] Status InitSdl();
    [[nodiscard]] Status ShutdownSdl() const;
    [[nodiscard]] Status InitImGui();
    [[nodiscard]] Status ShutdownImGui();
  };
}
#endif //WINDOW_HPP_04125118
