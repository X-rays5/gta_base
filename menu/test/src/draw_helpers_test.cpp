//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

#include "../../src/render/draw/draw_helpers.hpp"

namespace {
  using base::menu::render::draw_helpers::FitIntoBox;

  TEST(draw_helpers, fitting_content_keeps_its_aspect_ratio) {
    // 400x400px box, 200x100 content: the width is what runs out first, so the result is 400x200.
    const auto fitted = FitIntoBox({0.1F, 0.1F}, {0.4F, 0.4F}, {200.F, 100.F}, {1000.F, 1000.F});

    EXPECT_NEAR(fitted.size.x, 0.4F, 0.0001F);
    EXPECT_NEAR(fitted.size.y, 0.2F, 0.0001F);
    EXPECT_NEAR(fitted.size.x / fitted.size.y, 2.F, 0.0001F) << "Which is the content's own 2:1.";
  }

  TEST(draw_helpers, fitting_content_centres_it_in_the_box) {
    const auto fitted = FitIntoBox({0.1F, 0.1F}, {0.4F, 0.4F}, {200.F, 100.F}, {1000.F, 1000.F});

    EXPECT_NEAR(fitted.position.x, 0.1F, 0.0001F) << "It fills the box's width, so it starts there.";
    EXPECT_NEAR(fitted.position.y, 0.2F, 0.0001F) << "It leaves 0.1 of the 0.4 height above and below.";
  }

  TEST(draw_helpers, the_two_axes_are_measured_in_pixels_not_in_box_units) {
    // A 500x250px box is square in [0, 1] space but not on a 1000x500 screen, and square content
    // has to come out square: 250x250px rather than being stretched to fill the box.
    const auto fitted = FitIntoBox({0.F, 0.F}, {0.5F, 0.5F}, {100.F, 100.F}, {1000.F, 500.F});

    EXPECT_NEAR(fitted.size.x * 1000.F, 250.F, 0.01F);
    EXPECT_NEAR(fitted.size.y * 500.F, 250.F, 0.01F);
  }

  TEST(draw_helpers, content_is_scaled_up_to_fill_a_box_larger_than_it) {
    const auto fitted = FitIntoBox({0.F, 0.F}, {0.5F, 0.5F}, {10.F, 10.F}, {1000.F, 1000.F});

    EXPECT_NEAR(fitted.size.x, 0.5F, 0.0001F);
    EXPECT_NEAR(fitted.size.y, 0.5F, 0.0001F);
  }

  TEST(draw_helpers, nothing_to_fit_gives_nothing_to_draw) {
    const ImVec2 box_size{0.4F, 0.4F};
    const ImVec2 content_size{100.F, 100.F};
    const ImVec2 resolution{1000.F, 1000.F};

    EXPECT_FLOAT_EQ(FitIntoBox({0.F, 0.F}, {0.F, 0.F}, content_size, resolution).size.x, 0.F) << "No box.";
    EXPECT_FLOAT_EQ(FitIntoBox({0.F, 0.F}, box_size, {0.F, 0.F}, resolution).size.x, 0.F) << "No content.";
    EXPECT_FLOAT_EQ(FitIntoBox({0.F, 0.F}, box_size, content_size, {0.F, 0.F}).size.x, 0.F) << "No screen.";
  }
}
