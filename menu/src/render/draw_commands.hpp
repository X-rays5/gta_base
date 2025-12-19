//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#define GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#include <d3d12.h>
#include "draw_util.hpp"

// TODO: move most of this file to a cpp file

namespace base::menu::render {
  class BaseDrawCommand {
  public:
    BaseDrawCommand() = default;
    virtual ~BaseDrawCommand() = default;

    virtual void Draw() = 0;
  };

  class Rect : public BaseDrawCommand {
  public:
    Rect(const ImVec2 pos, const ImVec2 size, const ImU32 color) :
      pos_(pos), size_(size), color_(color) {}

    void Draw() override {
      util::GetDrawList()->AddRectFilled(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_);
    }

  protected:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class RectOutline final : public BaseDrawCommand {
  public:
    RectOutline(const ImVec2 pos, const ImVec2 size, const ImU32 color, const float thickness = 1.0F) :
      pos_(pos), size_(size), color_(color), thickness_(thickness) {}

    void Draw() override {
      util::GetDrawList()->AddRect(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_, 0.F, 0, thickness_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
    float thickness_;
  };

  class RectBorder final : public Rect {
  public:
    RectBorder(const ImVec2 pos, const ImVec2 size, const ImU32 background_color, const ImU32 border_color, const bool top, const bool bottom, const bool left, const bool right, const float thickness = 0.005F) :
      Rect(pos, size, background_color), border_color_(border_color), top_(top), bottom_(bottom), left_(left), right_(right), thickness_(thickness) {}

    void Draw() override {
      Rect::Draw();

      const auto draw_list = util::GetDrawList();
      if (top_)
        draw_list->AddLine(util::ScaleToScreen(pos_), util::ScaleToScreen(ImVec2(pos_.x + size_.x, pos_.y)), border_color_, thickness_);
      if (bottom_)
        draw_list->AddLine(util::ScaleToScreen(ImVec2(pos_.x, pos_.y + size_.y)), util::ScaleToScreen(ImVec2(pos_.x + size_.x, pos_.y + size_.y)), border_color_, thickness_);
      if (left_)
        draw_list->AddLine(util::ScaleToScreen(pos_), util::ScaleToScreen(ImVec2(pos_.x, pos_.y + size_.y)), border_color_, thickness_);
      if (right_)
        draw_list->AddLine(util::ScaleToScreen(ImVec2(pos_.x + size_.x, pos_.y)), util::ScaleToScreen(ImVec2(pos_.x + size_.x, pos_.y + size_.y)), border_color_, thickness_);
    }

  private:
    ImU32 border_color_;
    bool top_;
    bool bottom_;
    bool left_;
    bool right_;
    float thickness_;
  };

  class Text : public BaseDrawCommand {
  public:
    Text(const ImVec2 pos, const ImU32 color, std::string text, const float y_size_text, const bool right_align = false, const bool center_x = false, const bool center_y = false, const float max_width = 0.F, const std::size_t max_lines = 2, const ImFont* font = nullptr) :
      pos_(pos), color_(color), text_(std::move(text)), y_size_text_(y_size_text), right_align_(right_align), center_x_(center_x), center_y_(center_y), max_width_(max_width), max_lines_(max_lines), font_(font) {}

    void Draw() override {
      if (!font_) {
        font_ = ImGui::GetFont();
      }

      if (max_width_ > 0.F) {
        util::WordWrap(y_size_text_, text_, max_width_, max_lines_);
      }

      if (right_align_) {
        const ImVec2 text_size = util::CalcTextSize(font_, y_size_text_, text_);
        pos_.x -= text_size.x;
        pos_.y += text_size.y / 2;
      }

      if (!right_align_ && center_y_) {
        const ImVec2 text_size = util::CalcTextSize(font_, y_size_text_, text_);
        pos_.y += text_size.y / 2;
      }

      if (!right_align_ && center_x_) {
        const ImVec2 text_size = util::CalcTextSize(font_, y_size_text_, text_);
        pos_.x -= text_size.x / 2;
      }

      util::GetDrawList()->AddText(const_cast<ImFont*>(font_), util::ScaleFont(y_size_text_), util::ScaleToScreen(pos_), color_, text_.c_str());
    }

  protected:
    ImVec2 pos_;
    ImU32 color_;
    std::string text_;
    bool right_align_;
    bool center_x_;
    bool center_y_;
    float y_size_text_;
    float max_width_;
    std::size_t max_lines_;
    const ImFont* font_;
  };

  class TextBackground final : public Text {
  public:
    TextBackground(const ImVec2 pos, const ImU32 text_color, const ImU32 background_color, std::string text, const bool right_align, const bool center_x, const bool center_y, const float y_size_text, const float padding_side = 0.01F, const float padding_bottom_top = 0.01F, const bool border_top = false, const bool border_bottom = false, const bool border_left = false, const bool border_right = false, const ImU32 border_color = NULL, const float border_thickness = 1.F, const float max_width = 0.F, const std::size_t max_lines = 2, const ImFont* font = nullptr) :
      Text(pos, text_color, std::move(text), right_align, center_x, center_y, y_size_text, max_width, max_lines, font),
      rect_({}, {}, {}, {}, {}, {}, {}, {}) {
      ImVec2 rect_pos = pos_;
      rect_pos.x -= padding_side;
      rect_pos.y -= padding_bottom_top;

      ImVec2 rect_size = util::CalcTextSize(font_, y_size_text_, text_);
      rect_size.x += (padding_side * 2);
      rect_size.y += (padding_bottom_top * 2);

      rect_ = RectBorder(rect_pos, rect_size, background_color, border_color, border_top, border_bottom, border_left, border_right, border_thickness);
    }

    void Draw() override {
      rect_.Draw();
      Text::Draw();
    }

  private:
    RectBorder rect_;
  };

  class Image final : public BaseDrawCommand {
  public:
    Image(const D3D12_GPU_DESCRIPTOR_HANDLE texture_handle, const ImVec2 pos, const ImVec2 size, const ImU32 col = IM_COL32_WHITE, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1)) :
      texture_handle_(texture_handle), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col) {}

    void Draw() override {
      util::GetDrawList()->AddImage(texture_handle_.ptr, util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), uv_min_, uv_max_, col_);
    }

  private:
    D3D12_GPU_DESCRIPTOR_HANDLE texture_handle_;
    ImVec2 pos_;
    ImVec2 size_;
    ImVec2 uv_min_;
    ImVec2 uv_max_;
    ImU32 col_;
  };

  class RunRenderCode final : public BaseDrawCommand {
  public:
    explicit RunRenderCode(std::function<void()> render_code) :
      render_code_(std::move(render_code)) {}

    void Draw() override {
      render_code_();
    }

  private:
    std::function<void()> render_code_;
  };
}
#endif //GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
