//
// Created by xray on 07/09/2023.
//

#ifndef GTA_BASE_DRAW_UTIL_AE07DB6C33FB43E1B6FE62992EE1D737_HPP
#define GTA_BASE_DRAW_UTIL_AE07DB6C33FB43E1B6FE62992EE1D737_HPP
#include <cstdint>
#include <string>
#include <imgui/imgui.h>

namespace base::menu::render::draw_helpers {
  ImVec2 GetSize(ImVec2 pos, ImVec2 size);

  /// scale float in range [0, 1] to [0, screen_size]
  ImVec2 ScaleToScreen(ImVec2 xy);
  /// scale float in range [0, screen_size] to [0, 1]
  ImVec2 ScaleFromScreen(ImVec2 xy);

  float ScaleXToScreen(float x);
  float ScaleXFromScreen(float x);
  float ScaleYToScreen(float y);
  float ScaleYFromScreen(float y);
  ImVec2 ScaleSquare(float y);
  float ScaleFont(float size);

  ImVec2 CalcTextSizeRaw(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f);
  ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f);

  /// Where to draw content and how big, in the same space as the box it was fitted into.
  struct FittedRect {
    ImVec2 position;
    ImVec2 size;
  };

  /**
   * Scales content to fill as much of a box as it can without changing shape, and centres it.
   *
   * @param resolution Screen size in pixels. The box and the result are in [0, 1] screen space,
   *                   where the two axes hold different numbers of pixels, so an aspect ratio can
   *                   only be compared once both are measured in pixels.
   * @returns A zero size if the box, the content or the resolution has no extent to fit into.
   */
  FittedRect FitIntoBox(ImVec2 box_position, ImVec2 box_size, ImVec2 content_size, ImVec2 resolution);

  std::uint32_t WordWrap(float font_size, std::string& str, float max_x, std::size_t max_lines);

  ImDrawList* GetDrawList();

  class RotateVertices {
  public:
    void ImRotateStart();
    void ImRotateEnd(std::int32_t degrees);

  private:
    std::int32_t rotation_start_idx_ = -1;

  private:
    ImVec2 ImRotateCenter() const;
  };
}
#endif //GTA_BASE_DRAW_UTIL_AE07DB6C33FB43E1B6FE62992EE1D737_HPP
