//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

#include "../../src/render/draw/image/animated_image.hpp"

namespace {
  using base::menu::render::draw::AnimatedImage;

  std::vector<std::uint8_t> Frames(const std::uint32_t width, const std::uint32_t frame_height, const std::size_t count) {
    return std::vector<std::uint8_t>(static_cast<std::size_t>(width) * frame_height * count * 4);
  }

  TEST(animated_image, each_frame_is_shown_for_its_own_delay) {
    const AnimatedImage image(2, 2, Frames(2, 2, 3), {100, 200, 300});

    EXPECT_EQ(image.FrameAt(0), 0U);
    EXPECT_EQ(image.FrameAt(99), 0U);
    EXPECT_EQ(image.FrameAt(100), 1U) << "The second frame starts once the first one's delay has passed.";
    EXPECT_EQ(image.FrameAt(299), 1U);
    EXPECT_EQ(image.FrameAt(300), 2U);
    EXPECT_EQ(image.FrameAt(599), 2U);
  }

  TEST(animated_image, the_loop_wraps_at_the_total_duration) {
    const AnimatedImage image(2, 2, Frames(2, 2, 3), {100, 200, 300});

    EXPECT_EQ(image.FrameAt(600), 0U) << "600ms is one whole 100+200+300ms loop.";
    EXPECT_EQ(image.FrameAt(700), 1U) << "100ms into the second pass is the second frame.";
  }

  TEST(animated_image, a_frame_with_no_declared_delay_does_not_stall_the_loop) {
    // A GIF is allowed to declare 0, and a loop with no duration would have nothing to wrap
    // against. Browsers substitute 100ms, so the first frame lasts that rather than forever.
    const AnimatedImage image(2, 2, Frames(2, 2, 2), {0, 50});

    EXPECT_EQ(image.FrameAt(99), 0U);
    EXPECT_EQ(image.FrameAt(100), 1U);
    EXPECT_EQ(image.FrameAt(150), 0U) << "100 + 50ms is the whole loop.";
  }

  TEST(animated_image, uv_covers_only_the_frame_the_clock_lands_on) {
    const AnimatedImage image(2, 2, Frames(2, 2, 2), {100000, 100000});

    // The image has only just been built, so the clock is still inside the first frame.
    EXPECT_FLOAT_EQ(image.GetUvMin().y, 0.F);
    EXPECT_NEAR(image.GetUvMax().y, 0.5F, 0.0001F) << "Two frames stack to the whole texture, so each is half.";
  }

  TEST(animated_image, a_single_frame_image_stays_on_that_frame) {
    const AnimatedImage image(2, 2, Frames(2, 2, 1), {100});

    EXPECT_EQ(image.GetFrameCount(), 1U);
    EXPECT_EQ(image.FrameAt(10000), 0U);
    EXPECT_FLOAT_EQ(image.GetFrameSize().x, 2.F);
    EXPECT_FLOAT_EQ(image.GetFrameSize().y, 2.F);
  }
}
