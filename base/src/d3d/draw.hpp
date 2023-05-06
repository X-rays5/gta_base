//
// Created by X-ray on 07/02/22.
//

#pragma once
#include <cstddef>

#ifndef GTA_BASE_DRAW_HPP
#define GTA_BASE_DRAW_HPP
#include <string>
#include <memory>
#include <iostream>
#include <numeric>
#include <cassert>
#include <utility>
#include <stacktrace>
#include <d3d11.h>
#include <imgui.h>
#include "../misc/spinlock.hpp"
#include "renderer.hpp"

namespace gta_base::d3d::draw {
  FORCE_INLINE ImDrawList* GetDrawList() {
    return ImGui::GetForegroundDrawList();
  }

  FORCE_INLINE ImVec2 GetSize(ImVec2 pos, ImVec2 size) {
    return {pos.x + size.x, pos.y + size.y};
  }

  // scale float in range [0, 1] to [0, screen_size]
  FORCE_INLINE ImVec2 ScaleToScreen(ImVec2 xy) {
    if (xy.x < 0 || xy.x > 1 || xy.y < 0 || xy.y > 1) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
      LOG_WARN("ScaleToScreen: xy out of range: {} {}\n{}", xy.x, xy.y, std::to_string(std::stacktrace::current()));

      return {0, 0};
    }

    ImVec2 cur_res = ImGui::GetIO().DisplaySize;

    xy.x = (xy.x * cur_res.x);
    xy.y = (xy.y * cur_res.y);

    return xy;
  }

  // scale float in range [0, screen_size] to [0, 1]
  FORCE_INLINE ImVec2 ScaleFromScreen(ImVec2 xy) {
    ImVec2 cur_res = ImGui::GetIO().DisplaySize;
    if (cur_res.x == 0 || cur_res.y == 0) {
      return {}; // Happens when the window is minimized.
    }

    if (xy.x < 0 || xy.x > cur_res.x || xy.y < 0 || xy.y > cur_res.y) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
      LOG_WARN("ScaleFromScreen: xy out of range: {} {}\n {}", xy.x, xy.y, std::to_string(std::stacktrace::current()));

      return {0, 0};
    }

    xy.x = (xy.x / cur_res.x);
    xy.y = (xy.y / cur_res.y);

    return xy;
  }

  FORCE_INLINE float ScaleXToScreen(float x) {
    return ScaleToScreen({x, 0}).x;
  }

  FORCE_INLINE float ScaleXFromScreen(float x) {
    return ScaleFromScreen({x, 0}).x;
  }

  FORCE_INLINE float ScaleYToScreen(float y) {
    return ScaleToScreen({0, y}).y;
  }

  FORCE_INLINE float ScaleYFromScreen(float y) {
    return ScaleFromScreen({0, y}).y;
  }

  FORCE_INLINE ImVec2 ScaleSquare(float y) {
    auto tmp = ScaleYToScreen(y);
    return ScaleFromScreen({tmp, tmp});
  }

  FORCE_INLINE float ScaleFont(float size) {
    return ScaleYToScreen(size);
  }

  FORCE_INLINE ImVec2 CalcTextSizeRaw(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
    if (!font)
      font = kRENDERER->GetFont();

    if (!font) {
      LOG_CRITICAL("No valid fonts are loaded");
      return {};
    }

    ImVec2 text_size = font->CalcTextSizeA(ScaleFont(font_size), ImGui::GetIO().DisplaySize.x, wrap_width, text.c_str());
    text_size.x = ((float) (int) (text_size.x + 0.99999f));

    return text_size;
  }

  FORCE_INLINE ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
    if (text.empty())
      return {0, 0};

    return ScaleFromScreen(CalcTextSizeRaw(font, font_size, text, wrap_width));
  }

  FORCE_INLINE std::string WordWrapGetString(std::string* lines, std::uint32_t line_count) {
    std::string res;
    for (std::uint32_t i = 0; i < line_count; i++) {
      auto line = lines[i];
      if (line.empty())
        continue;

      while (line.back() == ' ') {
        line.pop_back();
      }
      res = fmt::format("{}{}\n", res, line);
    }

    return res;
  }

  // FIXME: Make it work with non monospaced fonts.
  // 2 lines take 0.01 ms in debug so it's fine for now
  inline std::uint32_t WordWrap(float font_size, std::string& str, float max_x, std::uint32_t max_lines) {
    auto real_max_x = ScaleXToScreen(max_x);

    if (str.empty() || CalcTextSizeRaw(kRENDERER->GetFont(), font_size, str).x <= real_max_x) {
      return 1; // Wrapping is not needed so just return that there is 1 line
    }

    auto char_x_size = CalcTextSizeRaw(kRENDERER->GetFont(), font_size, " ").x;
    auto chars_per_line = (std::uint32_t) (real_max_x / char_x_size);

    auto* lines = new std::string[max_lines];
    std::uint32_t line_count = 0;
    while (!str.empty()) {
      if (line_count >= max_lines) {
        auto last_line = lines[max_lines - 1];
        if (last_line.size() <= 3)
          break;

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

      if (str.empty() && CalcTextSizeRaw(kRENDERER->GetFont(), font_size, lines[line_count - 1]).x <= real_max_x) {
        break;
      }
    }

    str = WordWrapGetString(lines, line_count);
    delete[] lines;

    return line_count;
  }

  class Animate {
  public:
    Animate() = default;

    Animate(double start, double end, std::uint64_t duration) : start_(start), end_(end), duration_(duration) {}

    FORCE_INLINE double GetNow() {
      // use lerp to go from start to end
      curr_time_ += kRENDERER->GetDeltaTime();
      if (curr_time_ >= duration_) {
        return end_;
      }

      double percentage = ((double) (curr_time_ * 100) / (double) duration_) / 100;
      if (percentage >= 1)
        return end_;

      return std::lerp(start_, end_, percentage);
    }

  private:
    double start_{};
    double end_{};
    std::uint64_t curr_time_{};
    std::uint64_t duration_{};
  };

  class BaseDrawCommand {
  public:
    virtual ~BaseDrawCommand() = default;

    virtual void Draw() = 0;
  };

  // This can be used to execute code that should run on the render thread on render of a draw list
  // Also usable to draw ImGui windows
  class DrawCallback : public BaseDrawCommand {
  public:
    FORCE_INLINE explicit DrawCallback(std::function<void()> cb) : cb_(std::move(cb)) {}

    FORCE_INLINE void Draw() final {
      cb_();
    };

  private:
    std::function<void()> cb_;
  };

  class Rect : public BaseDrawCommand {
  public:
    FORCE_INLINE Rect(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color) {}

    FORCE_INLINE void Draw() final {
      GetDrawList()->AddRectFilled(ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), color_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class RectOutline : public BaseDrawCommand {
  public:
    FORCE_INLINE RectOutline(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color) {}

    FORCE_INLINE void Draw() final {
      GetDrawList()->AddRect(ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), color_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class Text : public BaseDrawCommand {
  public:
    FORCE_INLINE Text(ImVec2 pos, ImU32 color, std::string text, bool right_align, bool center, float y_size_text, const ImFont* font = nullptr) : pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_(center), y_size_text_(y_size_text), font_(font) {
    }

    FORCE_INLINE void Draw() final {
      if (!font_) {
        font_ = kRENDERER->GetFont();
      }

      if (right_align_) {
        ImVec2 text_size = CalcTextSize(font_, y_size_text_, text_);
        pos_.x -= text_size.x;
        pos_.y += (text_size.y / 2);
      } else if (center_) {
        ImVec2 text_size = CalcTextSize(font_, y_size_text_, text_);
        pos_.y += ((text_size.y / 2));
      }

      GetDrawList()->AddText(font_, ScaleFont(y_size_text_), ScaleToScreen(pos_), color_, text_.c_str());
    }

  private:
    ImVec2 pos_;
    ImU32 color_;
    std::string text_;
    bool right_align_;
    bool center_;
    float y_size_text_;
    const ImFont* font_;
  };

  class Image : public BaseDrawCommand {
  public:
    FORCE_INLINE Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, ImU32 col = IM_COL32_WHITE, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1)) : texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col) {
    }

    FORCE_INLINE void Draw() final {
      GetDrawList()->AddImage((void*) texture_, ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), uv_min_, uv_max_, col_);
    }

  private:
    ID3D11ShaderResourceView* texture_;
    ImVec2 pos_;
    ImVec2 size_;
    ImVec2 uv_min_;
    ImVec2 uv_max_;
    ImU32 col_;
  };

  class DrawList {
  public:
    explicit DrawList(std::size_t draw_command_buffer_count) {
      assert(draw_command_buffer_count >= 2);

      draw_lists_.resize(draw_command_buffer_count);
      draw_list_idx_.resize(draw_command_buffer_count);
      for (auto&& draw_list : draw_lists_) {
        draw_list.resize(initial_draw_commands);
      }
    }

    template<typename T>
    requires std::is_base_of_v<BaseDrawCommand, T>
    FORCE_INLINE void AddCommand(T command) {
      lock_.Lock();

      GetDrawListAt(GetWriteTarget())[GetWriteIdx()] = std::move(std::make_unique<T>(command));

      lock_.Unlock();
    }

    FORCE_INLINE void Draw() {
      misc::ScopedSpinlock lock(lock_);
      has_drawn_ = true;
      for (auto&& command : GetDrawListAt(GetRenderTarget())) {
        if (command != nullptr)
          command->Draw();
        else
          break;
      }
    }

    FORCE_INLINE void NextTargets() {
      lock_.Lock();
      has_drawn_ = false;
      NextRenderTarget();
      NextWriteTarget();
      lock_.Unlock();
    }

    [[nodiscard]] FORCE_INLINE bool HasDrawn() const {
      return has_drawn_;
    }

    [[nodiscard]] FORCE_INLINE std::size_t GetWriteTarget() const {
      return cur_write_target_;
    }

    [[nodiscard]] FORCE_INLINE std::size_t GetRenderTarget() const {
      return cur_render_target_;
    }

  private:
    constexpr static const std::size_t initial_draw_commands = 4096;

    using draw_list_t = std::vector<std::unique_ptr<BaseDrawCommand>>;

    misc::RecursiveSpinlock lock_;

    /// id of draw_list currently being written to.
    std::size_t cur_write_target_ = 1;
    /// id of draw_list commands should currently be rendered from
    std::size_t cur_render_target_ = 0;

    std::atomic<bool> has_drawn_ = false;

    std::vector<draw_list_t> draw_lists_;
    /// Stores the next position in a draw_list to read/write.
    /// Access with the draw_list id.
    std::vector<std::size_t> draw_list_idx_;

  private:
    FORCE_INLINE void ClearDrawList(std::size_t idx) {
      GetDrawListAt(GetWriteTarget()).clear();
      GetDrawListAt(GetWriteTarget()).resize(initial_draw_commands);
    }

    FORCE_INLINE void NextRenderTarget() {
      cur_render_target_ += 1;
      if (cur_render_target_ >= draw_lists_.size())
        cur_render_target_ = 0;
    }

    FORCE_INLINE void NextWriteTarget() {
      cur_write_target_ += 1;
      if (cur_write_target_ >= draw_lists_.size())
        cur_write_target_ = 0;

      ClearDrawList(cur_write_target_);
    }

    FORCE_INLINE draw_list_t& GetDrawListAt(std::size_t idx) {
      LOG_CRITICAL_CONDITIONAL(idx >= draw_lists_.size(), "Requested draw list at idx: {}. Max index: {}", idx, draw_lists_.size() - 1);

      return draw_lists_[idx];
    }

    FORCE_INLINE std::size_t GetWriteIdx() {
      return draw_list_idx_[GetWriteTarget()];
    }
  };
}
#endif //GTA_BASE_DRAW_HPP