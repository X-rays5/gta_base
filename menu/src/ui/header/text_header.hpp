//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include "base_header.hpp"

namespace base::menu::ui {
  class TextHeader final : public BaseHeader {
    public:

    TextHeader() : BaseHeader{} {}

    virtual ~TextHeader() = default;

    virtual void Render(render::DrawQueueBuffer* draw_queue, ImVec2 dimensions, ImVec2 position, const std::float_t color_alpha, MenuRenderProperties& render_properties) override {
      RenderBackground(draw_queue, dimensions, position, color_alpha, render_properties);

      const auto text_pos = ImVec2(position.x + dimensions.x / 2, position.y + dimensions.y / 2);
      draw_queue->AddCommand(render::Text(text_pos, GetAlpha(render_properties.theme->text_props.text_color, color_alpha), common::globals::kBASE_NAME, font_size_, false, true, true));
    }

  private:
    inline static UiSize font_size_{0.02f};
  };
}
