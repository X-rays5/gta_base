//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#define GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#include <d3d11.h>
#include "draw_helpers.hpp"
#include "../../../../imfont/src/imfont.hpp"

// TODO: move most of this file to a cpp file

namespace base::menu::render {
  class BaseDrawCommand {
  public:
    BaseDrawCommand() = default;
    virtual ~BaseDrawCommand() = default;

    virtual void Draw() const = 0;
  };

  class Rect : public BaseDrawCommand {
  public:
    Rect(const ImVec2 pos, const ImVec2 size, const ImU32 color) :
      pos_(pos), size_(size), color_(color) {}

    virtual void Draw() const override {
      draw_helpers::GetDrawList()->AddRectFilled(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), color_);
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

    virtual void Draw() const override {
      draw_helpers::GetDrawList()->AddRect(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), color_, 0.F,NULL, thickness_);
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

    virtual void Draw() const override {
      Rect::Draw();

      const auto draw_list = draw_helpers::GetDrawList();
      if (top_) {
        draw_list->AddRectFilled(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, {size_.x, thickness_})), border_color_);
      }
      if (bottom_) {
        const ImVec2 bottom_pos =  {pos_.x, pos_.y + size_.y - thickness_};
        draw_list->AddRectFilled(draw_helpers::ScaleToScreen(bottom_pos), draw_helpers::ScaleToScreen(draw_helpers::GetSize(bottom_pos, {size_.x, thickness_})), border_color_);
      }
      if (left_) {
        draw_list->AddRectFilled(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, {thickness_, size_.y})), border_color_);
      }
      if (right_) {
        const ImVec2 right_pos = {pos_.x + size_.x - thickness_, pos_.y};
        draw_list->AddRectFilled(draw_helpers::ScaleToScreen(right_pos), draw_helpers::ScaleToScreen(draw_helpers::GetSize(right_pos, {thickness_, size_.y})), border_color_);
      }
    }

  private:
    ImU32 border_color_;
    float thickness_;
    bool top_;
    bool bottom_;
    bool left_;
    bool right_;
  };

  class PushFont final : public BaseDrawCommand {
  public:
    explicit PushFont(const std::string& font_name_) : font_name_(font_name_) {}

    virtual void Draw() const override {
      imfont::kMANAGER->PushFont(font_name_);
    }

  private:
    std::string font_name_;
  };

  class PopFont final : public BaseDrawCommand {
  public:
    PopFont() = default;

    virtual void Draw() const override {
      imfont::Manager::PopFont();
    }
  };

  class Text : public BaseDrawCommand {
  public:
    Text(const ImVec2 pos, const ImU32 color, std::string text, const float y_size_text, const bool right_align = false, const bool center_x = false, const bool center_y = false, const float max_width = 0.F, const std::size_t max_lines = 2, const ImFont* font = nullptr) :
      pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_x_(center_x), center_y_(center_y), y_size_text_(y_size_text), max_width_(max_width), max_lines_(max_lines), font_(font) {}

    virtual void Draw() const override {
      std::string tmp_text = text_;
      ImVec2 tmp_pos = pos_;

      auto font = const_cast<ImFont*>(font_);
      if (!font_) {
        font = ImGui::GetFont();
      }

      if (max_width_ > 0.F) {
        draw_helpers::WordWrap(y_size_text_, tmp_text, max_width_, max_lines_);
      }

      if (right_align_) {
        const ImVec2 text_size = draw_helpers::CalcTextSize(font, y_size_text_, tmp_text);
        tmp_pos.x -= text_size.x;
      }

      if (center_y_) {
        const ImVec2 text_size = draw_helpers::CalcTextSize(font, y_size_text_, tmp_text);
        tmp_pos.y -= text_size.y / 2;
      }

      if (!right_align_ && center_x_) {
        const ImVec2 text_size = draw_helpers::CalcTextSize(font, y_size_text_, tmp_text);
        tmp_pos.x -= text_size.x / 2;
      }

      draw_helpers::GetDrawList()->AddText(font, draw_helpers::ScaleFont(y_size_text_), draw_helpers::ScaleToScreen(tmp_pos), color_, tmp_text.c_str());
    }

  protected:
    const ImVec2 pos_;
    const ImU32 color_;
    const bool right_align_;
    const bool center_x_;
    const bool center_y_;
    const std::string text_;
    const float y_size_text_;
    const float max_width_;
    const std::size_t max_lines_;
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

      ImVec2 rect_size = draw_helpers::CalcTextSize(font_, y_size_text_, text_);
      rect_size.x += (padding_side * 2);
      rect_size.y += (padding_bottom_top * 2);

      rect_ = RectBorder(rect_pos, rect_size, background_color, border_color, border_top, border_bottom, border_left, border_right, border_thickness);
    }

    virtual void Draw() const override {
      rect_.Draw();
      Text::Draw();
    }

  private:
    RectBorder rect_;
  };

  class Image final : public BaseDrawCommand {
  public:
    Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, ImU32 col = IM_COL32_WHITE, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1)) :
      texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col) {}

    virtual void Draw() const override {
      draw_helpers::GetDrawList()->AddImage(reinterpret_cast<ImTextureID>(texture_), draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), uv_min_, uv_max_, col_);
    }

  private:
    ID3D11ShaderResourceView* texture_;
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

    virtual void Draw() const override {
      render_code_();
    }

  private:
    std::function<void()> render_code_;
  };
}
#endif //GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
