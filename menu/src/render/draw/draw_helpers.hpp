//
// Created by xray on 07/09/2023.
//

#ifndef GTA_BASE_DRAW_UTIL_AE07DB6C33FB43E1B6FE62992EE1D737_HPP
#define GTA_BASE_DRAW_UTIL_AE07DB6C33FB43E1B6FE62992EE1D737_HPP
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

  // TODO: Make a non monospace version of this.
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
