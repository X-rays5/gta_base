//
// Created by xray on 07/09/2023.
//

#include <numbers>
#include "draw_helpers.hpp"
#include "../renderer.hpp"

namespace base::menu::render::draw_helpers {
  namespace {
    FORCE_INLINE std::string WordWrapGetString(const std::string* lines, const std::uint32_t line_count) {
      std::string res;
      for (std::uint32_t i = 0; i < line_count; i++) {
        auto line = lines[i];
        while (line.back() == ' ') {
          line.pop_back();
        }
        res = fmt::format("{}{}\n", res, line);
      }

      return res;
    }
  }

  [[nodiscard]] ImDrawList* GetDrawList() {
    return ImGui::GetForegroundDrawList();
  }

  [[nodiscard]] ImVec2 GetSize(const ImVec2 pos, const ImVec2 size) {
    return {pos.x + size.x, pos.y + size.y};
  }

  // scale float in range [0, 1] to [0, screen_size]
  [[nodiscard]] ImVec2 ScaleToScreen(ImVec2 xy) {
    xy.x = std::clamp(xy.x, 0.0F, 1.0F);
    xy.y = std::clamp(xy.y, 0.0F, 1.0F);

    ImVec2 cur_res{0, 0};
    if (kRENDERER)
      cur_res = kRENDERER->GetResolution();

    xy.x = std::floor(xy.x * cur_res.x);
    xy.y = std::floor(xy.y * cur_res.y);

    return xy;
  }

  /// scale float in range [0, screen_size] to [0, 1]
  [[nodiscard]] ImVec2 ScaleFromScreen(ImVec2 xy) {
    ImVec2 cur_res{0, 0};
    if (kRENDERER)
      cur_res = kRENDERER->GetResolution();

    if (cur_res.x == 0 || cur_res.y == 0) {
      LOG_DEBUG("Cannot scale from screen, resolution is 0x0");
      return {0,0};
    }

    xy.x = std::clamp(xy.x / cur_res.x, 0.0F, 1.0F);
    xy.y = std::clamp(xy.y / cur_res.y, 0.0F, 1.0F);

    return xy;
  }

  [[nodiscard]] float ScaleXToScreen(float x) {
    return ScaleToScreen({x, 0}).x;
  }

  [[nodiscard]] float ScaleXFromScreen(float x) {
    return ScaleFromScreen({x, 0}).x;
  }

  [[nodiscard]] float ScaleYToScreen(float y) {
    return ScaleToScreen({0, y}).y;
  }

  [[nodiscard]] float ScaleYFromScreen(float y) {
    return ScaleFromScreen({0, y}).y;
  }

  [[nodiscard]] ImVec2 ScaleSquare(float y) {
    auto tmp = ScaleYToScreen(y);
    return ScaleFromScreen({tmp, tmp});
  }

  [[nodiscard]] float ScaleFont(float size) {
    return ScaleYToScreen(size);
  }

  [[nodiscard]] ImVec2 CalcTextSizeRaw(const ImFont* font, const float font_size, const std::string& text, float wrap_width) {
    if (!font) {
      font = ImGui::GetFont();
    }

    ImVec2 text_size = const_cast<ImFont*>(font)->CalcTextSizeA(ScaleFont(font_size), ImGui::GetIO().DisplaySize.x, wrap_width, text.c_str());
    text_size.x = text_size.x + 0.99999F;

    return text_size;
  }

  [[nodiscard]] ImVec2 CalcTextSize(const ImFont* font, const float font_size, const std::string& text, const float wrap_width) {
    return ScaleFromScreen(CalcTextSizeRaw(font, font_size, text, wrap_width));
  }

  std::uint32_t WordWrap(const float font_size, std::string& str, const float max_x, const std::size_t max_lines) {
    const auto real_max_x = ScaleXToScreen(max_x);

    if (str.empty() || CalcTextSizeRaw(ImGui::GetFont(), font_size, str).x <= real_max_x) {
      return 1; // Wrapping is not needed, so just return that there is 1 line
    }

    std::vector<std::string> lines;
    lines.reserve(max_lines);

    while (!str.empty() && lines.size() < max_lines) {
      std::size_t best_break_pos = 0;
      std::size_t last_space_pos = std::string::npos;

      // Build the line character by character, measuring width as we go
      for (std::size_t i = 0; i < str.size(); ++i) {
        // Check if current character is a space
        if (str[i] == ' ') {
          last_space_pos = i;
        }

        // Measure text from start to current position
        std::string current_line = str.substr(0, i + 1);
        float line_width = CalcTextSizeRaw(ImGui::GetFont(), font_size, current_line).x;

        // If we've exceeded the max width
        if (line_width > real_max_x) {
          // If we found a space before this point, break there
          if (last_space_pos != std::string::npos && last_space_pos > 0) {
            best_break_pos = last_space_pos + 1; // Include the space
          } else {
            // No space found, break at current position
            best_break_pos = i;
          }
          break;
        }

        best_break_pos = i + 1; // Update best position as we go
      }

      // If we didn't find a break point, it means the entire remaining string fits
      if (best_break_pos == 0) {
        best_break_pos = str.size();
      }

      // Extract the line and remove trailing spaces
      std::string line = str.substr(0, best_break_pos);
      while (!line.empty() && line.back() == ' ') {
        line.pop_back();
      }

      if (!line.empty()) {
        lines.push_back(line);
      }

      // Remove the processed part from str
      str.erase(0, best_break_pos);

      // Skip leading spaces in the remaining string
      while (!str.empty() && str.front() == ' ') {
        str.erase(0, 1);
      }
    }

    // If we hit max lines and there's still text, add ellipsis to the last line
    if (!str.empty() && !lines.empty()) {
      auto& last_line = lines.back();
      if (last_line.size() >= 3) {
        last_line.replace(last_line.end() - 3, last_line.end(), "...");
      }
    }

    str = WordWrapGetString(lines);

    return static_cast<std::uint32_t>(lines.size());
  }

  void RotateVertices::ImRotateStart() {
    rotation_start_idx_ = GetDrawList()->VtxBuffer.Size;
  }

  ImVec2 RotateVertices::ImRotateCenter() const {
    if (rotation_start_idx_ < 0) {
      LOG_ERROR("Rotation start index is not set, cannot calculate center");
      return {0, 0};
    }

    auto& vtx_buffer = GetDrawList()->VtxBuffer;
    if (vtx_buffer.empty()) {
      LOG_ERROR("Vertex buffer is empty, cannot calculate center");
      return {0, 0};
    }

    ImVec2 l(FLT_MAX, FLT_MAX);
    ImVec2 u(-FLT_MAX, -FLT_MAX); // bounds

    for (std::int32_t i = rotation_start_idx_; i < vtx_buffer.Size; i++) {
      const ImVec2& pos = vtx_buffer[i].pos;
      l.x = std::min(l.x, pos.x);
      l.y = std::min(l.y, pos.y);
      u.x = std::max(u.x, pos.x);
      u.y = std::max(u.y, pos.y);
    }
    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
  }

  void RotateVertices::ImRotateEnd(const std::int32_t degrees) {
    if (rotation_start_idx_ < 0) {
      LOG_ERROR("Rotation start index is not set, cannot rotate vertices");
      return;
    }

    const ImVec2 center = ImRotateCenter();

    auto& vtx_buffer = GetDrawList()->VtxBuffer;
    for (int i = rotation_start_idx_; i < vtx_buffer.Size; i++) {
      const ImVec2 pos = vtx_buffer[i].pos;
      const float rad = degrees * std::numbers::pi_v<std::float_t> / 180.0f;
      const float cos_rad = cos(rad);
      const float sin_rad = sin(rad);

      // Rotate around the center
      vtx_buffer[i].pos.x = center.x + (pos.x - center.x) * cos_rad - (pos.y - center.y) * sin_rad;
      vtx_buffer[i].pos.y = center.y + (pos.x - center.x) * sin_rad + (pos.y - center.y) * cos_rad;

      rotation_start_idx_ = -1; // Reset the rotation start index after rotating
    }
  }

}
