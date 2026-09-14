//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

// theme.hpp is written against the menu precompiled header and names Status without including it, and
// text_header.hpp reaches the menu's name the same way. A test is not that translation unit, so
// base-common comes first, as the other tests here do.
#include <base-common/globals.hpp>
#include <base-common/util/result.hpp>

#include "../../src/ui/header/header_type.hpp"
#include "../../src/ui/header/image_header.hpp"
#include "../../src/ui/header/text_header.hpp"

namespace {
  using base::menu::ui::HeaderType;
  using base::menu::ui::ImageHeader;
  using base::menu::ui::MakeHeader;
  using base::menu::ui::TextHeader;

  /// What the theme asks for is the header that is built, and the type is the only thing that decides
  /// it: an image that is not there is not an invitation to build another type instead. The headers are
  /// told apart by what they are, since what they draw needs a renderer.
  TEST(header_type, the_text_type_builds_the_text_header) {
    const auto header = MakeHeader(HeaderType::kText, "");

    ASSERT_NE(header, nullptr);
    EXPECT_NE(dynamic_cast<TextHeader*>(header.get()), nullptr);
  }

  TEST(header_type, the_image_type_builds_the_image_header) {
    const auto header = MakeHeader(HeaderType::kImage, "");

    ASSERT_NE(header, nullptr);
    EXPECT_NE(dynamic_cast<ImageHeader*>(header.get()), nullptr);
  }

  /// The image header is still the image header when its image is not there or was never set - that
  /// header drawing nothing is the point - and the text header is not what it turns into.
  TEST(header_type, the_image_type_is_built_even_without_an_image_to_draw) {
    const auto header = MakeHeader(HeaderType::kImage, "no_such_header_image.png");

    ASSERT_NE(header, nullptr);
    EXPECT_NE(dynamic_cast<ImageHeader*>(header.get()), nullptr);
    EXPECT_EQ(dynamic_cast<TextHeader*>(header.get()), nullptr);
  }

  /// A type is stored as a number, so a theme file can hold one this build has no header for. It gets a
  /// header rather than none.
  TEST(header_type, a_type_this_build_has_no_header_for_builds_the_default) {
    const auto header = MakeHeader(static_cast<HeaderType>(99), "");

    ASSERT_NE(header, nullptr);
    EXPECT_NE(dynamic_cast<TextHeader*>(header.get()), nullptr);
  }
}
