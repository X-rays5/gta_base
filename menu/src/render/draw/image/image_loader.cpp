//
// Created by X-ray on 13/09/2026.
//

#include "image_loader.hpp"

#include <climits>
#include <fstream>
#include <stb_image.h>
#include "animated_image.hpp"
#include "static_image.hpp"

namespace base::menu::render::draw::image_loader {
  namespace {
    constexpr int kChannels = 4;
    constexpr int kMaxSize = INT_MAX;

    /// stb only recognises a GIF for this, and only stbi_load_gif_from_memory hands back the
    /// per-frame delays.
    bool IsGif(const std::span<const std::uint8_t> data) {
      if (data.size() < 6) {
        return false;
      }

      return memcmp(data.data(), "GIF87a", 6) == 0 || memcmp(data.data(), "GIF89a", 6) == 0;
    }

    StatusOr<std::shared_ptr<BaseImage>> LoadGif(const std::span<const std::uint8_t> data) {
      int* delays = nullptr;
      int width = 0;
      int height = 0;
      int frame_count = 0;
      int channels = 0;

      stbi_uc* pixels = stbi_load_gif_from_memory(data.data(), static_cast<int>(data.size()), &delays, &width, &height, &frame_count, &channels, kChannels);
      if (pixels == nullptr) {
        return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Failed to decode GIF: {}", stbi_failure_reason());
      }

      std::vector<std::uint8_t> frames(pixels, pixels + static_cast<std::size_t>(width) * height * frame_count * kChannels);
      std::vector<std::uint32_t> frame_delays(static_cast<std::size_t>(frame_count));
      for (std::size_t i = 0; i < frame_delays.size(); ++i) {
        frame_delays[i] = static_cast<std::uint32_t>(std::max(delays[i], 0));
      }

      stbi_image_free(pixels);
      stbi_image_free(delays);

      if (frame_count <= 1) {
        return std::make_shared<StaticImage>(width, height, std::move(frames));
      }

      return std::make_shared<AnimatedImage>(width, static_cast<std::uint32_t>(height), std::move(frames), std::move(frame_delays));
    }

    StatusOr<std::shared_ptr<BaseImage>> LoadStatic(const std::span<const std::uint8_t> data) {
      int width = 0;
      int height = 0;
      int channels = 0;

      stbi_uc* pixels = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &width, &height, &channels, kChannels);
      if (pixels == nullptr) {
        return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Failed to decode image: {}", stbi_failure_reason());
      }

      std::vector<std::uint8_t> rgba(pixels, pixels + static_cast<std::size_t>(width) * height * kChannels);
      stbi_image_free(pixels);

      return std::make_shared<StaticImage>(width, height, std::move(rgba));
    }
  }

  StatusOr<std::shared_ptr<BaseImage>> LoadImageFromMemory(const std::span<const std::uint8_t> data) {
    if (data.empty()) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("No image data to decode");
    }

    if (data.size() > kMaxSize) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Image of {} bytes is too large to decode", data.size());
    }

    return IsGif(data) ? LoadGif(data) : LoadStatic(data);
  }

  StatusOr<std::shared_ptr<BaseImage>> LoadImage(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to open image: {}", path.string());
    }

    const auto size = file.tellg();
    if (size <= 0) {
      return MakeFailure<ResultCode::kIO_ERROR>("Image is empty: {}", path.string());
    }

    std::vector<std::uint8_t> data(static_cast<std::size_t>(size));
    file.seekg(0);
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to read image: {}", path.string());
    }

    return LoadImageFromMemory(data);
  }
}
