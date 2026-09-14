//
// Created by X-ray on 13/09/2026.
//

#pragma once
#include <chrono>
#include "base_image.hpp"

namespace base::menu::render::draw {
  /**
   * Frames of equal size stacked vertically in one texture, the layout
   * stbi_load_gif_from_memory decodes a GIF into.
   *
   * Timing is read from the clock on every query rather than advanced per frame: the same image
   * can be drawn more than once in a frame, and counting those would run the animation fast.
   */
  class AnimatedImage final : public BaseImage {
  public:
    /// @param frames RGBA, width * (frame_height * frame_count) * 4 bytes, top frame first.
    /// @param frame_delays_ms One delay per frame, as the GIF declares them.
    AnimatedImage(std::uint32_t width, std::uint32_t frame_height, std::vector<std::uint8_t> frames,
                  std::vector<std::uint32_t> frame_delays_ms);

    [[nodiscard]] ImVec2 GetUvMin() const override;
    [[nodiscard]] ImVec2 GetUvMax() const override;

    [[nodiscard]] ImVec2 GetFrameSize() const override {
      return {static_cast<float>(GetWidth()), static_cast<float>(frame_height_)};
    }

    [[nodiscard]] std::size_t GetFrameCount() const {
      return frame_delays_ms_.size();
    }

    /// Restarts the loop from the first frame.
    void Reset();

    /// Which frame is showing elapsed_ms into the loop. Wraps around.
    [[nodiscard]] std::size_t FrameAt(std::uint64_t elapsed_ms) const;

  private:
    /// A GIF is allowed to declare no delay at all, which would leave the loop with no duration
    /// to wrap against. Browsers substitute 100ms for that, so do the same.
    static constexpr std::uint32_t kDefaultFrameDelayMs = 100;

    [[nodiscard]] std::uint64_t ElapsedMs() const;

    std::uint32_t frame_height_;
    std::vector<std::uint32_t> frame_delays_ms_;
    std::uint64_t total_duration_ms_ = 0;
    std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();
  };
}
