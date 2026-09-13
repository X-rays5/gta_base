//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include "base_header.hpp"
#include "../../render/draw/draw_queue.hpp"

namespace base::menu::ui {
  class TextHeader final : public BaseHeader {
    public:

    TextHeader() : BaseHeader{} {}

    virtual ~TextHeader() = default;

    virtual void Render(render::DrawQueueBuffer* draw_queue, ImVec2 dimensions, ImVec2 position, const std::float_t color_alpha, MenuRenderProperties& render_properties) override {
      position.x -= render::draw_helpers::ScaleSquare(render_properties.seperator_height).x;
      dimensions.x += render::draw_helpers::ScaleSquare(render_properties.seperator_height).x * 2;

      const auto background_color = GetAlpha(render_properties.theme->background_color, color_alpha);
      const auto seperator_color = GetAlpha(render_properties.theme->seperator_color, color_alpha);
      const auto text_color = GetAlpha(render_properties.theme->text_props.text_color, color_alpha);

      draw_queue->AddCommand(render::RectBorder(position, dimensions, background_color, seperator_color,
        true, false, true, true, render_properties.seperator_height));

      const auto text_pos = ImVec2(position.x + dimensions.x / 2, position.y + dimensions.y / 2);
      draw_queue->AddCommand(render::Text(text_pos, text_color, common::globals::kBASE_NAME, font_size_, false, true, true));
    }

  private:
    inline static UiSize font_size_{0.02f};
  };
}
