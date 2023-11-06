//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#define GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#include "draw_util.hpp"
#include <d3d11.h>

// TODO: move most of this file to a cpp file

namespace base::render {
  class BaseDrawCommand {
  public:
    BaseDrawCommand() = default;
    virtual ~BaseDrawCommand() = 0;

    virtual void Draw() = 0;
  };

  class Rect : public BaseDrawCommand {
  public:
    Rect(ImVec2 pos, ImVec2 size, ImU32 color) :
    pos_(pos), size_(size), color_(color) {}

    void Draw() override {
      util::GetDrawList()->AddRectFilled(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_);
    }

  protected:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class RectOutline : public BaseDrawCommand {
  public:
    RectOutline(ImVec2 pos, ImVec2 size, ImU32 color, float thickness = 1.0f) :
    pos_(pos), size_(size), color_(color), thickness_(thickness) {}

    void Draw() final {
      util::GetDrawList()->AddRect(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_, 0.f,NULL, thickness_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
    float thickness_;
  };

  class RectBorder : Rect {
  public:
    RectBorder(ImVec2 pos, ImVec2 size, ImU32 background_color, ImU32 border_color, bool top, bool bottom, bool left, bool right, float thickness = 0.005f) :
    Rect(pos, size, background_color), border_color_(border_color), top_(top), bottom_(bottom), left_(left), right_(right), thickness_(thickness) {}

    void Draw() final {
      Rect::Draw();

      auto draw_list = util::GetDrawList();

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
    Text(ImVec2 pos, ImU32 color, std::string text, bool right_align, bool center, float y_size_text, float max_width = 0.f, std::size_t max_lines = 2, const ImFont* font = nullptr) :
    pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_(center), y_size_text_(y_size_text), max_width_(max_width), max_lines_(max_lines), font_(font) {}

    void Draw() override {
      if (!font_) {
        font_ = ImGui::GetFont();
      }

      if (max_width_ > 0.f) {
        util::WordWrap(y_size_text_, text_, max_width_, max_lines_);
      }

      if (right_align_) {
        ImVec2 text_size = util::CalcTextSize(font_, y_size_text_, text_);
        pos_.x -= text_size.x;
        pos_.y += (text_size.y / 2);
      } else if (center_) {
        ImVec2 text_size = util::CalcTextSize(font_, y_size_text_, text_);
        pos_.y += ((text_size.y / 2));
      }

      util::GetDrawList()->AddText(font_, util::ScaleFont(y_size_text_), util::ScaleToScreen(pos_), color_, text_.c_str());
    }

  protected:
    ImVec2 pos_;
    ImU32 color_;
    std::string text_;
    bool right_align_;
    bool center_;
    float y_size_text_;
    float max_width_;
    std::size_t max_lines_;
    const ImFont* font_;
  };

  class TextBackground : Text {
  public:
    TextBackground(ImVec2 pos, ImU32 text_color, ImU32 background_color, std::string text, bool right_align, bool center, float y_size_text, float padding_side = 0.01f, float padding_bottom_top = 0.01f, bool border_top = false, bool border_bottom = false, bool border_left = false, bool border_right = false, ImU32 border_color = NULL, float border_thickness = 1.f, float max_width = 0.f, std::size_t max_lines = 2, const ImFont* font = nullptr) :
    Text(pos, text_color, std::move(text), right_align, center, y_size_text, max_width, max_lines, font),
    rect_({}, {}, {}, {}, {}, {}, {}, {})
    {
      ImVec2 rect_pos = pos_;
      rect_pos.x -= padding_side;
      rect_pos.y -= padding_bottom_top;

      ImVec2 rect_size = util::CalcTextSize(font_, y_size_text_, text_);
      rect_size.x += (padding_side * 2);
      rect_size.y += (padding_bottom_top * 2);

      rect_ = RectBorder(rect_pos, rect_size, background_color, border_color, border_top, border_bottom, border_left, border_right, border_thickness);
    }

    void Draw() final {
      rect_.Draw();
      Text::Draw();
    }

  private:
    RectBorder rect_;
  };

  class Image : public BaseDrawCommand {
  public:
    Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, ImU32 col = IM_COL32_WHITE, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1)) :
    texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col) {}

    void Draw() final {
      util::GetDrawList()->AddImage((void*) texture_, util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), uv_min_, uv_max_, col_);
    }

  private:
    ID3D11ShaderResourceView* texture_;
    ImVec2 pos_;
    ImVec2 size_;
    ImVec2 uv_min_;
    ImVec2 uv_max_;
    ImU32 col_;
  };

  class RunRenderCode : public BaseDrawCommand {
  public:
    explicit RunRenderCode(std::function<void()> render_code) :
    render_code_(std::move(render_code)) {}

    void Draw() final {
      render_code_();
    }

  private:
    std::function<void()> render_code_;
  };
}
#endif //GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
