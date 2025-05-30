//
// Created by X-ray on 04/05/2024.
//

#include "notification.hpp"
#include "../../render/draw.hpp"

namespace base::ui::notification {
  ImColor Notification::GetNotificationColor(const Type type) {
    switch (type) {
    case Type::Info:
      return info_color_;
    case Type::Warning:
      return warning_color_;
    case Type::Error:
      return error_color_;
    default:
      LOG_ERROR("Unknown notification type: {}", static_cast<int>(type));
      return info_color_;
    }
  }

  std::float_t Notification::Draw(menu::render::DrawQueueBuffer* draw_queue_buffer, const bool right_align, std::float_t y_offset) {
    constexpr std::float_t y_size = .05f;

    const auto notification_x_pos = right_align ? 1.f - notification_width_ - x_margin_ : 0 + x_margin_;
    y_offset += y_margin_;

    // if (!notification_animation_) {
    //   notification_animation_ = std::make_unique<render::animate::LerpWaitReturn<std::float_t>>(right_align ? 1.f : 0.f, notification_x_pos, slide_duration_ms_, duration_ms - slide_duration_ms_);
    // }

    //auto x = notification_animation_->Animate(delta_time);
    draw_queue_buffer->AddCommand(menu::render::Rect({notification_x_pos, y_offset}, {notification_width_, y_size}, ImColor(0, 0, 0, notification_transparency_)));
    draw_queue_buffer->AddCommand(menu::render::Rect({notification_x_pos, y_offset}, {notification_width_, top_bar_thickness_}, notification_color_));
    draw_queue_buffer->AddCommand(menu::render::Text({notification_x_pos + text_margin_, y_offset + text_margin_}, ImColor(255, 255, 255), title_, title_text_size_));

    return y_size + y_margin_;
  }
}
