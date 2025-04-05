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
      // use lerp to go from start to end
      curr_time_ += delta_time;
      if (curr_time_ >= duration_) {
        return end_;
      }

      float percentage = (static_cast<float>(curr_time_ * 100) / static_cast<float>(duration_)) / 100;
      if (percentage >= 1)
        return end_;

      return std::lerp(start_, end_, percentage);
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
      lerp_{start, end, duration_ms},
      end_{end},
      wait_ms_{wait_ms} {
      LOG_DEBUG("LerpWaitReturn: start: {}, end: {}, duration: {}, wait: {}", start, end, duration_ms, wait_ms);
    }

    [[nodiscard]] T Animate(std::size_t delta_time) {
      return lerp_.Animate(delta_time);
    }

  private:
    bool wait_time_elapsed_{false};
    Lerp<T> lerp_;
    std::size_t wait_ms_;
    T end_;
  };
}

#endif //ANIMATE_HPP_05193205
