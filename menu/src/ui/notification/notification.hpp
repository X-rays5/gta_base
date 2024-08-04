//
// Created by X-ray on 04/05/2024.
//

#ifndef NOTIFICATION_HPP_05174124
#define NOTIFICATION_HPP_05174124
#include "../../render/animate.hpp"

namespace base::render {
  class DrawQueueBuffer;
}


namespace base::ui::notification {
  class Manager;

  enum class Type {
    Info,
    Warning,
    Error
  };

  class Notification {
  private:
    Notification(const Type type, const std::string& title, const std::string& message) : type_(type), title_(title), message_(message) {}

    void Draw(render::DrawQueueBuffer* draw_queue_buffer, std::size_t delta_time, bool right_align, std::float_t y_offset, std::size_t duration_ms);

    friend class Manager;

  private:
    Type type_;
    std::string title_;
    std::string message_;
    std::unique_ptr<render::animate::LerpWaitReturn<std::float_t>> notification_animation_;

    static constexpr auto notification_width_ = 0.10f;
  };
};

#endif //NOTIFICATION_HPP_05174124
