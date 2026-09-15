//
// Created by X-ray on 13/09/2026.
//

#include "animated_image.hpp"

namespace base::menu::render::draw {
  AnimatedImage::AnimatedImage(const std::uint32_t width, const std::uint32_t frame_height, std::vector<std::uint8_t> frames,
                               std::vector<std::uint32_t> frame_delays_ms) :
    BaseImage(width, frame_height * static_cast<std::uint32_t>(std::max<std::size_t>(frame_delays_ms.size(), 1)), std::move(frames)),
    frame_height_(frame_height),
    frame_delays_ms_(std::move(frame_delays_ms)) {
    for (auto& delay : frame_delays_ms_) {
      if (delay == 0) {
        delay = kDefaultFrameDelayMs;
      }
      total_duration_ms_ += delay;
    }
  }

  void AnimatedImage::Reset() {
    start_time_ = std::chrono::steady_clock::now();
  }

  std::size_t AnimatedImage::FrameAt(const std::uint64_t elapsed_ms) const {
    if (frame_delays_ms_.size() <= 1 || total_duration_ms_ == 0) {
      return 0;
    }

    std::uint64_t remaining = elapsed_ms % total_duration_ms_;
    for (std::size_t frame = 0; frame < frame_delays_ms_.size(); ++frame) {
      if (remaining < frame_delays_ms_[frame]) {
        return frame;
      }
      remaining -= frame_delays_ms_[frame];
    }

    return frame_delays_ms_.size() - 1;
  }

  ImVec2 AnimatedImage::GetUvMin() const {
    if (frame_delays_ms_.empty()) {
      return {0.F, 0.F};
    }

    return {0.F, static_cast<float>(FrameAt(ElapsedMs())) / static_cast<float>(frame_delays_ms_.size())};
  }

  ImVec2 AnimatedImage::GetUvMax() const {
    if (frame_delays_ms_.empty()) {
      return {1.F, 1.F};
    }

    return {1.F, static_cast<float>(FrameAt(ElapsedMs()) + 1) / static_cast<float>(frame_delays_ms_.size())};
  }

  std::uint64_t AnimatedImage::ElapsedMs() const {
    const auto elapsed = std::chrono::steady_clock::now() - start_time_;
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
  }
}
