//
// Created by X-ray on 14/09/2026.
//

#include "header_image.hpp"
#include <algorithm>
#include <array>
#include <base-common/fs/vfs.hpp>
#include "../../render/draw/image/image_loader.hpp"

#undef LoadImage

namespace base::menu::ui::header_image {
  namespace {
    /// What stb_image reads and a banner is plausibly stored as. Anything else in the images
    /// directory is somebody's own file, not a header.
    constexpr std::array<std::string_view, 6> kImageExtensions{".png", ".jpg", ".jpeg", ".bmp", ".tga", ".gif"};

    std::string LowerExtension(const std::filesystem::path& path) {
      std::string extension = path.extension().string();
      std::ranges::transform(extension, extension.begin(), [](const unsigned char character) {
        return static_cast<char>(std::tolower(character));
      });
      return extension;
    }
  }

  std::filesystem::path ResolvePath(const std::string_view configured) {
    if (configured.empty()) {
      return {};
    }

    const std::filesystem::path path{configured};
    return path.is_absolute() ? path : common::fs::vfs::GetImageDir() / path;
  }

  std::vector<std::string> ListAvailable(const std::filesystem::path& dir) {
    std::vector<std::string> names;

    std::error_code error;
    if (!std::filesystem::is_directory(dir, error)) {
      return names;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir, error)) {
      if (entry.is_regular_file() && std::ranges::find(kImageExtensions, LowerExtension(entry.path())) != kImageExtensions.end()) {
        names.push_back(entry.path().filename().string());
      }
    }

    std::ranges::sort(names);
    return names;
  }

  std::shared_ptr<render::draw::BaseImage> Load(const std::string_view configured) {
    if (configured.empty()) {
      return nullptr;
    }

    const auto path = ResolvePath(configured);
    auto image = render::draw::image_loader::LoadImage(path);
    if (!image) {
      LOG_WARN("No header image drawn for '{}': {}", path.string(), image.error().GetResultMessage());
      return nullptr;
    }

    LOG_DEBUG("Loaded header image from '{}', {}x{} pixels", path.string(), image.value()->GetWidth(), image.value()->GetHeight());
    return image.value();
  }
}
