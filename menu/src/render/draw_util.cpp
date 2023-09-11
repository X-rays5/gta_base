//
// Created by xray on 07/09/2023.
//

#include "draw_util.hpp"
#include <stacktrace>

namespace base::render::util {
  namespace {
    FORCE_INLINE std::string WordWrapGetString(std::string* lines, std::uint32_t line_count) {
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

  ImDrawList* GetDrawList() {
    return ImGui::GetForegroundDrawList();
  }

  ImVec2 GetSize(ImVec2 pos, ImVec2 size) {
    return {pos.x + size.x, pos.y + size.y};
  }

  // scale float in range [0, 1] to [0, screen_size]
  ImVec2 ScaleToScreen(ImVec2 xy) {
    if (xy.x < 0 || xy.x > 1 || xy.y < 0 || xy.y > 1) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
      LOG_WARN("ScaleToScreen: xy out of range: {} {}\n{}", xy.x, xy.y, std::to_string(std::stacktrace::current()));
    }

    ImVec2 cur_res = ImGui::GetIO().DisplaySize;

    xy.x = (xy.x * cur_res.x);
    xy.y = (xy.y * cur_res.y);

    return xy;
  }

  ImVec2 ScaleFromScreen(ImVec2 xy) {
    ImVec2 cur_res = ImGui::GetIO().DisplaySize;
    if (cur_res.x == 0 || cur_res.y == 0) {
      return {}; // Happens when the window is minimized.
    }

    if (xy.x < 0 || xy.x > cur_res.x || xy.y < 0 || xy.y > cur_res.y) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
      LOG_WARN("ScaleFromScreen: xy out of range: {} {}\n {}", xy.x, xy.y, std::to_string(std::stacktrace::current()));
    }

    xy.x = (xy.x / cur_res.x);
    xy.y = (xy.y / cur_res.y);

    return xy;
  }

  float ScaleXToScreen(float x) {
    return ScaleToScreen({x, 0}).x;
  }

  float ScaleXFromScreen(float x) {
    return ScaleFromScreen({x, 0}).x;
  }

  float ScaleYToScreen(float y) {
    return ScaleToScreen({0, y}).y;
  }

  float ScaleYFromScreen(float y) {
    return ScaleFromScreen({0, y}).y;
  }

  ImVec2 ScaleSquare(float y) {
    auto tmp = ScaleYToScreen(y);
    return ScaleFromScreen({tmp, tmp});
  }

  float ScaleFont(float size) {
    return ScaleYToScreen(size);
  }

  ImVec2 CalcTextSizeRaw(const ImFont* font, float font_size, const std::string& text, float wrap_width) {
    if (!font)
      font = ImGui::GetFont();

    ImVec2 text_size = font->CalcTextSizeA(ScaleFont(font_size), ImGui::GetIO().DisplaySize.x, wrap_width, text.c_str());
    text_size.x = ((float) (int) (text_size.x + 0.99999f));

    return text_size;
  }

  ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width) {
    return ScaleFromScreen(CalcTextSizeRaw(font, font_size, text, wrap_width));
  }

  std::uint32_t WordWrap(float font_size, std::string& str, float max_x, std::uint32_t max_lines) {
    auto real_max_x = ScaleXToScreen(max_x);

    if (str.empty() || CalcTextSizeRaw(ImGui::GetFont(), font_size, str).x <= real_max_x) {
      return 1; // Wrapping is not needed so just return that there is 1 line
    }

    auto char_x_size = CalcTextSizeRaw(ImGui::GetFont(), font_size, " ").x;
    auto chars_per_line = (std::uint32_t) (real_max_x / char_x_size);

    auto* lines = new std::string[max_lines];
    std::uint32_t line_count = 0;
    while (!str.empty()) {
      if (line_count >= max_lines) {
        auto last_line = lines[max_lines - 1];
        last_line.replace(last_line.end() - 3, last_line.end(), "...");
        lines[max_lines - 1] = last_line;
        break;
      }

      std::size_t pos = chars_per_line < str.size() ? str.rfind(' ', chars_per_line) : str.size();
      if (pos != std::string::npos) {
        lines[line_count] = str.substr(0, pos + 1);
        str.erase(0, pos + 1);
      } else {
        lines[line_count] = str.substr(0, chars_per_line);
        str.erase(0, chars_per_line);
      }
      line_count += 1;

      if (str.empty() && CalcTextSizeRaw(ImGui::GetFont(), font_size, lines[line_count - 1]).x <= real_max_x) {
        break;
      }
    }

    str = WordWrapGetString(lines, line_count);
    delete[] lines;

    return line_count;
  }
}