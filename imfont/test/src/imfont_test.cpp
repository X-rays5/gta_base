//
// Created by X-ray on 06/04/2025.
//

#include <array>
#include <battery/embed.hpp>
#include <gtest/gtest.h>
#include <imfont/imfont.hpp>
#include "test_globals.hpp"

TEST(ImFont, LoadDefaultFont) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);

  imfont::Manager manager;
}

TEST(ImFont, LoadFromDisk) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);

  imfont::Manager manager;

  EXPECT_TRUE(manager.LoadFontFromDisk("arialfa", "C:\\Windows\\Fonts\\arial.ttf"));
  EXPECT_TRUE(manager.LoadFontFromDisk("arial", "C:\\Windows\\Fonts\\arial.ttf", 20.f, false));
}

TEST(ImFont, LoadFromDiskBadPath) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);

  imfont::Manager manager;

  EXPECT_FALSE(manager.LoadFontFromDisk("not_a_font", "C:\\Windows\\Fonts\\not_a_font.ttf"));
  EXPECT_FALSE(manager.LoadFontFromDisk("not_a_font", "C:\\Windows\\Fonts\\not_a_font.ttf", 20.f, false));
}

TEST(ImFont, LoadFromMemory) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);

  imfont::Manager manager;

  const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
  EXPECT_TRUE(manager.LoadFontFromMemory("robototestfa", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size()));
  EXPECT_TRUE(manager.LoadFontFromMemory("robototest", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size(), 20.f, false));
}

TEST(ImFont, LoadFontInvalidName) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);
  imfont::Manager manager;
  const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
  EXPECT_FALSE(manager.LoadFontFromMemory("", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size()));
}

TEST(ImFont, LoadFontZeroSize) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);
  imfont::Manager manager;
  const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
  EXPECT_FALSE(manager.LoadFontFromMemory("robototest", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size(), 0.f, false));
}

TEST(ImFont, LoadFontAlreadyLoaded) {
  base::common::concurrency::ScopedSpinlock lock(kRENDER_LOCK);
  imfont::Manager manager;
  const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
  EXPECT_TRUE(manager.LoadFontFromMemory("robototest", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size()));
  EXPECT_FALSE(manager.LoadFontFromMemory("robototest", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size()));
}