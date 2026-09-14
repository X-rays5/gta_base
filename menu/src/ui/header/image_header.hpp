//
// Created by X-ray on 14/09/2026.
//

#pragma once
#include "base_header.hpp"

namespace base::menu::ui {
  /// A header that is the image, centred in the header box.
  class ImageHeader final : public BaseHeader {
  public:
    explicit ImageHeader(std::shared_ptr<render::draw::BaseImage> image) : image_{std::move(image)},
      BaseHeader() {}

    virtual ~ImageHeader() override = default;

    virtual void Render(render::DrawQueueBuffer* draw_queue, const ImVec2 dimensions, const ImVec2 position, const std::float_t color_alpha, MenuRenderProperties& render_properties) override {
      if (!image_ || dimensions.x <= 0.F || dimensions.y <= 0.F) {
        return;
      }

      draw_queue->AddCommand(render::Image(image_, position, dimensions, GetAlpha(RgbColor(255, 255, 255), color_alpha)));
    }

  private:
    std::shared_ptr<render::draw::BaseImage> image_;
  };
}
