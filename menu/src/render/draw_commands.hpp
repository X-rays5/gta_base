//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#define GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#include "draw_util.hpp"
#include <d3d11.h>

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

    void Draw() final {
      util::GetDrawList()->AddRectFilled(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class RectOutline : public BaseDrawCommand {
  public:
    RectOutline(ImVec2 pos, ImVec2 size, ImU32 color) :
    pos_(pos), size_(size), color_(color) {}

    void Draw() final {
      util::GetDrawList()->AddRect(util::ScaleToScreen(pos_), util::ScaleToScreen(util::GetSize(pos_, size_)), color_);
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class Text : public BaseDrawCommand {
  public:
    Text(ImVec2 pos, ImU32 color, std::string text, bool right_align, bool center, float y_size_text, const ImFont* font = nullptr) :
    pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_(center), y_size_text_(y_size_text), font_(font) {}

    void Draw() final {
      if (!font_) {
        font_ = ImGui::GetFont();
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
    RunRenderCode(std::function<void()> render_code) :
    render_code_(std::move(render_code)) {}

    void Draw() final {
      render_code_();
    }

  private:
    std::function<void()> render_code_;
  };
}
#endif //GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
