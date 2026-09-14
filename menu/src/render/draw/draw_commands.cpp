//
// Created by X-ray on 10/09/2026.
//

#include "draw_commands.hpp"

#include <imfont/imfont.hpp>

namespace base::menu::render {
  Rect::Rect(const ImVec2 pos, const ImVec2 size, const ImU32 color) :
    pos_(pos), size_(size), color_(color) {}

  void Rect::Draw() const {
    draw_helpers::GetDrawList()->AddRectFilled(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), color_);
  }

  RectOutline::RectOutline(const ImVec2 pos, const ImVec2 size, const ImU32 color, const float thickness) :
    pos_(pos), size_(size), color_(color), thickness_(thickness) {}

  void RectOutline::Draw() const {
    draw_helpers::GetDrawList()->AddRect(draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), color_, 0.F, NULL, thickness_);
  }

  RectBorder::RectBorder(const ImVec2 pos, const ImVec2 size, const ImU32 background_color, const ImU32 border_color, const bool top, const bool bottom, const bool left, const bool right, const float thickness) :
    Rect(pos, size, background_color), border_color_(border_color), thickness_(thickness), top_(top), bottom_(bottom), left_(left), right_(right) {
  }

  void RectBorder::Draw() const {
    Rect::Draw();

    const auto draw_list = draw_helpers::GetDrawList();

    // Calculate consistent thickness in screen pixels
    const float thickness_x_screen = draw_helpers::ScaleXToScreen(thickness_);
    const float thickness_y_screen = draw_helpers::ScaleYToScreen(thickness_);

    // Use the smaller value to ensure all borders match
    const float thickness_screen = std::min(thickness_x_screen, thickness_y_screen);

    // For converting back to normalized coordinates
    const float thickness_x_norm = thickness_screen / draw_helpers::ScaleXToScreen(1.0f);
    const float thickness_y_norm = thickness_screen / draw_helpers::ScaleYToScreen(1.0f);

    if (top_) {
      const auto top_pos = pos_;
      const auto top_size_end = draw_helpers::GetSize(top_pos, {size_.x, thickness_y_norm});
      draw_list->AddRectFilled(draw_helpers::ScaleToScreen(top_pos), draw_helpers::ScaleToScreen(top_size_end), border_color_);
    }
    if (bottom_) {
      const auto bottom_pos = ImVec2(pos_.x, pos_.y + size_.y - thickness_y_norm);
      const auto bottom_size_end = draw_helpers::GetSize(bottom_pos, {size_.x, thickness_y_norm});
      draw_list->AddRectFilled(draw_helpers::ScaleToScreen(bottom_pos), draw_helpers::ScaleToScreen(bottom_size_end), border_color_);
    }
    if (left_) {
      const auto left_pos = pos_;
      const auto left_size_end = draw_helpers::GetSize(left_pos, {thickness_x_norm, size_.y});
      draw_list->AddRectFilled(draw_helpers::ScaleToScreen(left_pos), draw_helpers::ScaleToScreen(left_size_end), border_color_);
    }
    if (right_) {
      const auto right_pos = ImVec2(pos_.x + size_.x - thickness_x_norm, pos_.y);
      const auto right_size_end = draw_helpers::GetSize(right_pos, {thickness_x_norm, size_.y});
      draw_list->AddRectFilled(draw_helpers::ScaleToScreen(right_pos), draw_helpers::ScaleToScreen(right_size_end), border_color_);
    }
  }

  PushFont::PushFont(const std::string& font_name_) : font_name_(font_name_) {}

  void PushFont::Draw() const {
    imfont::kMANAGER->PushFont(font_name_);
  }

  void PopFont::Draw() const {
    imfont::Manager::PopFont();
  }

  Text::Text(const ImVec2 pos, const ImU32 color, std::string text, const float y_size_text, const bool right_align, const bool center_x, const bool center_y, const float max_width, const std::size_t max_lines, const ImFont* font) :
    pos_(pos), color_(color), text_(std::move(text)), y_size_text_(y_size_text), right_align_(right_align), center_x_(center_x), center_y_(center_y), max_width_(max_width), max_lines_(max_lines), font_(font) {}

  void Text::Draw() const {
    auto font = const_cast<ImFont*>(font_);
    std::string tmp_text = text_;
    ImVec2 tmp_pos = pos_;

    if (!font) {
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

  TextRotate::TextRotate(const ImVec2 pos, const ImU32 text_color, std::string text, const bool right_align, const bool center_x, const bool center_y, const float y_size_text, const std::int32_t rotation_angle, const float max_width, const std::size_t max_lines, const ImFont* font) :
    Text(pos, text_color, std::move(text), y_size_text, right_align, center_x, center_y, max_width, max_lines, font),
    rotation_angle_(rotation_angle) {}

  void TextRotate::Draw() const {
    draw_helpers::RotateVertices rotate;
    rotate.ImRotateStart();
    Text::Draw();
    rotate.ImRotateEnd(rotation_angle_);
  }

  TextBackground::TextBackground(const ImVec2 pos, const ImU32 text_color, const ImU32 background_color, std::string text, const bool right_align, const bool center_x, const bool center_y, const float y_size_text, const float padding_side, const float padding_bottom_top, const bool border_top, const bool border_bottom, const bool border_left, const bool border_right, const ImU32 border_color, const float border_thickness, const float max_width, const std::size_t max_lines, const ImFont* font) :
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

  void TextBackground::Draw() const {
    rect_.Draw();
    Text::Draw();
  }

  Image::Image(std::shared_ptr<draw::BaseImage> image, const ImVec2 pos, const ImVec2 size, const ImU32 col) :
    image_(std::move(image)), pos_(pos), size_(size), col_(col) {}

  void Image::Draw() const {
    if (!image_) {
      LOG_ERROR("Image::Draw called with null image_");
      return;
    }

    // Uploads on first draw, which is also the first point the device is guaranteed to exist.
    const ImTextureID texture = image_->GetTextureId();
    if (texture == ImTextureID_Invalid) {
      LOG_ERROR("Image::Draw called with invalid texture");
      return;
    }

    draw_helpers::GetDrawList()->AddImage(texture, draw_helpers::ScaleToScreen(pos_), draw_helpers::ScaleToScreen(draw_helpers::GetSize(pos_, size_)), image_->GetUvMin(), image_->GetUvMax(), col_);
  }

  RunRenderCode::RunRenderCode(std::function<void()> render_code) :
    render_code_(std::move(render_code)) {}

  void RunRenderCode::Draw() const {
    render_code_();
  }
}
