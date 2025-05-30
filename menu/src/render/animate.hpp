//
// Created by X-ray on 04/05/2024.
//

#ifndef ANIMATE_HPP_05193205
#define ANIMATE_HPP_05193205
#include <tweeny.h>

namespace base::render::animate {
  template <typename T>
    requires std::is_arithmetic_v<T>
  class Lerp {
  public:
    Lerp(T start, T end, const std::size_t duration_ms) :
      start_{start},
      end_{end},
      duration_{duration_ms} {}

    [[nodiscard]] T Animate(const std::size_t delta_time) {
      curr_time_ += delta_time;
      if (curr_time_ >= duration_) {
        return end_; // Ensure it reaches the end value precisely
      }

      // Calculate the interpolation factor (t) between 0.0 and 1.0
      float t = static_cast<float>(curr_time_) / duration_;
      return std::lerp(start_, end_, t);
    }

    [[nodiscard]] T GetStart() {
      return start_;
    }

    [[nodiscard]] T GetEnd() {
      return end_;
    }

    [[nodiscard]] std::size_t GetCurrTime() const {
      return curr_time_;
    }

    [[nodiscard]] std::size_t GetDuration() const {
      return duration_;
    }

  private:
    T start_{};
    T end_{};
    std::size_t curr_time_{};
    std::size_t duration_{};
  };

  template <typename T>
    requires std::is_arithmetic_v<T>
  class LerpWaitReturn {
  public:
    LerpWaitReturn(T start, T end, std::size_t duration_ms, const std::size_t wait_ms) :
      lerp_start_{start, end, duration_ms},
      lerp_end_{end, start, duration_ms},
      wait_ms_{wait_ms},
      start_{start},
      end_{end} {}

    [[nodiscard]] T Animate(std::size_t delta_time) {
      if (waiting_) {
        curr_wait_time_ += delta_time;
        if (curr_wait_time_ >= wait_ms_) {
          waiting_ = false;
          first_phase_ = false;
          curr_wait_time_ = 0;
          return lerp_end_.Animate(delta_time);
        }

        return end_;
      }

      if (first_phase_) {
        T result = lerp_start_.Animate(delta_time);
        if (result == end_) {
          waiting_ = true;
          curr_wait_time_ = 0;
        }
        return result;
      }

      return lerp_end_.Animate(delta_time);
    }

  private:
    std::size_t curr_wait_time_{0};
    Lerp<T> lerp_start_;
    Lerp<T> lerp_end_;
    std::size_t wait_ms_;
    T start_;
    T end_;
    bool waiting_{false};
    bool first_phase_{true};
  };
}

#endif //ANIMATE_HPP_05193205
