//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

// theme.hpp is written against the menu precompiled header and names Status without including it.
// A test is not that translation unit, so base-common comes first, as the other tests here do.
#include <base-common/util/result.hpp>

#include "../../src/render/draw/draw_queue.hpp"
#include "../../src/render/draw/image/static_image.hpp"
#include "../../src/ui/header/image_header.hpp"

namespace {
  using base::menu::render::draw::StaticImage;
  using base::menu::render::DrawQueueBuffer;
  using base::menu::ui::ImageHeader;
  using base::menu::ui::MenuRenderProperties;

  std::shared_ptr<StaticImage> MakeImage(const std::uint32_t width, const std::uint32_t height) {
    return std::make_shared<StaticImage>(width, height, std::vector<std::uint8_t>(static_cast<std::size_t>(width) * height * 4));
  }

  /// The drawing itself needs a renderer, so what the header draws is read here as what it queues: the
  /// box it shares with every header, and the image over it.
  TEST(image_header, an_image_fills_the_header_box) {
    DrawQueueBuffer draw_queue;
    MenuRenderProperties properties;

    ImageHeader header(MakeImage(4, 4));
    header.Render(&draw_queue, {0.16F, 0.1F}, {0.005F, 0.005F}, 1.F, properties);

    EXPECT_EQ(draw_queue.CommandCount(), 2U);
  }

  /// The null image the renderer hands over when a theme names no image, or names one that does not
  /// load. Nothing goes in its place - no text, no placeholder: the box it is set into is all that is
  /// left, which is what the header being empty looks like.
  TEST(image_header, rendering_without_an_image_draws_nothing_in_the_box) {
    DrawQueueBuffer draw_queue;
    MenuRenderProperties properties;

    ImageHeader empty_header(nullptr);
    empty_header.Render(&draw_queue, {0.16F, 0.1F}, {0.005F, 0.005F}, 0.5F, properties);

    EXPECT_EQ(draw_queue.CommandCount(), 1U);
  }
}
