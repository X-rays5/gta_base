//
// Created by X-ray on 13/09/2026.
//

#pragma once

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
  };
}