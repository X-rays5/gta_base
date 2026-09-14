//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

#include <atomic>
#include <fstream>
#include <random>

// header_image names Status and the images directory without including them, as the menu's headers do
// under its precompiled header. A test is not that translation unit, so base-common goes first.
#include <base-common/fs/vfs.hpp>
#include <base-common/util/result.hpp>

#include "../../src/render/draw/image/base_image.hpp"
#include "../../src/ui/header/header_image.hpp"

namespace {
  using base::menu::render::draw::BaseImage;
  using base::menu::ui::header_image::ListAvailable;
  using base::menu::ui::header_image::Load;
  using base::menu::ui::header_image::ResolvePath;

  /// A name unique across processes, so that two tests running at once cannot write each other's files:
  /// ctest discovers one entry per test and is free to run those in parallel.
  std::string UniqueName(const std::string& base) {
    static std::atomic<int> counter{0};
    return base + "_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1));
  }

  /// A scratch directory of image files, removed with the test that made it.
  class ImageDir {
  public:
    ImageDir() {
      dir_ = std::filesystem::temp_directory_path() / ("header_image_" + UniqueName("dir"));
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
      std::filesystem::create_directories(dir_);
    }

    ~ImageDir() {
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
    }

    ImageDir(const ImageDir&) = delete;
    ImageDir(ImageDir&&) = delete;
    ImageDir& operator=(const ImageDir&) = delete;
    ImageDir& operator=(ImageDir&&) = delete;

    std::filesystem::path Write(const std::string& name, const std::string& bytes) const {
      const auto path = dir_ / name;
      std::ofstream file(path, std::ios::binary | std::ios::trunc);
      file << bytes;
      return path;
    }

    [[nodiscard]] const std::filesystem::path& Get() const {
      return dir_;
    }

  private:
    std::filesystem::path dir_;
  };

  /// The smallest image stb reads that is worth spelling out: a 1x1 binary PPM. A PNG would have to be
  /// a checked-in file or a block of escaped bytes for the same coverage.
  const std::string kTinyPpm = "P6\n1 1\n255\n" + std::string("\xff\x00\x00", 3);

  TEST(header_image, an_absolute_path_is_used_as_it_is) {
    const auto path = (std::filesystem::temp_directory_path() / "somewhere" / "banner.png").string();

    EXPECT_EQ(ResolvePath(path), std::filesystem::path(path));
  }

  TEST(header_image, a_bare_name_is_looked_up_in_the_images_directory) {
    EXPECT_EQ(ResolvePath("banner.png"), base::common::fs::vfs::GetImageDir() / "banner.png");
  }

  TEST(header_image, no_setting_is_no_path) {
    EXPECT_TRUE(ResolvePath("").empty());
  }

  TEST(header_image, only_images_are_listed_in_order) {
    const ImageDir dir;
    dir.Write("shot.jpg", "jpeg");
    dir.Write("banner.png", "png");
    dir.Write("wide.PNG", "png");
    dir.Write("notes.txt", "not an image");
    dir.Write("noext", "not an image either");
    std::filesystem::create_directories(dir.Get() / "sub.png");

    EXPECT_EQ(ListAvailable(dir.Get()), (std::vector<std::string>{"banner.png", "shot.jpg", "wide.PNG"}));
  }

  TEST(header_image, a_directory_that_is_not_there_lists_nothing) {
    EXPECT_TRUE(ListAvailable(std::filesystem::temp_directory_path() / "gta_base_no_such_image_dir").empty());
  }

  TEST(header_image, nothing_configured_loads_no_image) {
    EXPECT_EQ(Load(""), nullptr);
  }

  TEST(header_image, an_image_that_is_not_there_loads_nothing) {
    const ImageDir dir;

    // Null is the fallback the header is built on: an image that is not there draws nothing rather
    // than turning the header back into the text one.
    EXPECT_EQ(Load((dir.Get() / "missing.png").string()), nullptr);
  }

  TEST(header_image, a_readable_image_loads) {
    const ImageDir dir;
    const auto path = dir.Write("tiny.ppm", kTinyPpm);

    const std::shared_ptr<BaseImage> image = Load(path.string());
    ASSERT_NE(image, nullptr);

    const auto size = image->GetFrameSize();
    EXPECT_FLOAT_EQ(size.x, 1.F);
    EXPECT_FLOAT_EQ(size.y, 1.F);
  }
}
