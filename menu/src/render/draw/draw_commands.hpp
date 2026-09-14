//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#define GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
#include "draw_helpers.hpp"
#include "image/base_image.hpp"

namespace base::menu::render {
  class BaseDrawCommand {
  public:
    BaseDrawCommand() = default;
    virtual ~BaseDrawCommand() = default;

    virtual void Draw() const = 0;
  };

  class Rect : public BaseDrawCommand {
  public:
    Rect(ImVec2 pos, ImVec2 size, ImU32 color);

    void Draw() const override;

  protected:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
  };

  class RectOutline final : public BaseDrawCommand {
  public:
    RectOutline(ImVec2 pos, ImVec2 size, ImU32 color, float thickness = 1.0F);

    void Draw() const override;

  private:
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 color_;
    float thickness_;
  };

  class RectBorder final : public Rect {
  public:
    RectBorder(ImVec2 pos, ImVec2 size, ImU32 background_color, ImU32 border_color, bool top, bool bottom, bool left, bool right, float thickness = 0.005F);

    void Draw() const override;

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
    explicit PushFont(const std::string& font_name_);

    void Draw() const override;

  private:
    std::string font_name_;
  };

  class PopFont final : public BaseDrawCommand {
  public:
    PopFont() = default;

    void Draw() const override;
  };

  class Text : public BaseDrawCommand {
  public:
    Text(const ImVec2 pos, const ImU32 color, std::string text, const float y_size_text, const bool right_align = false, const bool center_x = false, const bool center_y = false, const float max_width = 0.F, const std::size_t max_lines = 2, const ImFont* font = nullptr);

    void Draw() const override;

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

  class TextRotate final : public Text {
  public:
    TextRotate(ImVec2 pos, ImU32 text_color, std::string text, bool right_align, bool center_x, bool center_y, float y_size_text, std::int32_t rotation_angle, float max_width = 0.F, std::size_t max_lines = 2, const ImFont* font = nullptr);

    void Draw() const override;

  private:
    const std::int32_t rotation_angle_;
  };

  class TextBackground final : public Text {
  public:
    TextBackground(ImVec2 pos, ImU32 text_color, ImU32 background_color, std::string text, bool right_align, bool center_x, bool center_y, float y_size_text, float padding_side = 0.01F, float padding_bottom_top = 0.01F, bool border_top = false, bool border_bottom = false, bool border_left = false, bool border_right = false, ImU32 border_color = NULL, float border_thickness = 1.F, float max_width = 0.F, std::size_t max_lines = 2, const ImFont* font = nullptr);

    void Draw() const override;

  private:
    RectBorder rect_;
  };

  class Image final : public BaseDrawCommand {
  public:
    Image(std::shared_ptr<draw::BaseImage> image, ImVec2 pos, ImVec2 size, ImU32 col = IM_COL32_WHITE);

    void Draw() const override;

  private:
    std::shared_ptr<draw::BaseImage> image_;
    ImVec2 pos_;
    ImVec2 size_;
    ImU32 col_;
  };

  class RunRenderCode final : public BaseDrawCommand {
  public:
    explicit RunRenderCode(std::function<void()> render_code);

    void Draw() const override;

  private:
    std::function<void()> render_code_;
  };
}
#endif //GTA_BASE_DRAW_COMMANDS_545AB8D13AD244EE82FA159E81A729AD_HPP
