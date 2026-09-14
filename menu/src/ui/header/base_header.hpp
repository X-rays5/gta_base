//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include <memory>
#include "../theme.hpp"
#include "../../render/draw/draw_queue.hpp"
#include "../../render/draw/image/base_image.hpp"

namespace base::menu::ui {
  class BaseHeader {
  public:
    virtual ~BaseHeader() = default;

    BaseHeader(const BaseHeader&) = delete;
    BaseHeader(BaseHeader&&) = delete;
    BaseHeader& operator=(const BaseHeader&) = delete;
    BaseHeader& operator=(BaseHeader&&) = delete;

    virtual void Render(render::DrawQueueBuffer* draw_queue, ImVec2 dimensions, ImVec2 position, std::float_t color_alpha, MenuRenderProperties& render_properties) = 0;

  protected:
    BaseHeader() = default;

    static RgbColor GetAlpha(RgbColor color, const std::float_t color_alpha) {
      color.a = static_cast<std::uint8_t>(color.a * color_alpha);
      return color;
    }

    /**
     * Draws the background and separator every header shares, widening both by one separator
     * height so the border sits outside the box the caller asked for.
     *
     * @note position and dimensions are updated in place: content belongs inside the drawn box,
     *       not the one that was passed in.
     */
    static void RenderBackground(render::DrawQueueBuffer* draw_queue, ImVec2& dimensions, ImVec2& position, const std::float_t color_alpha, const MenuRenderProperties& render_properties) {
      draw_queue->AddCommand(render::RectBorder(position, dimensions, GetAlpha(render_properties.theme->background_color, color_alpha),
        GetAlpha(render_properties.theme->seperator_color, color_alpha), true, false, true, true, render_properties.seperator_height));
    }
  };
}
