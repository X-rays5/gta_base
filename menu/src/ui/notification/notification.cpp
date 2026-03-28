//
// Created by X-ray on 04/05/2024.
//

#include "notification.hpp"
#include <base-common/util/time.hpp>
#include "../menu_renderer.hpp"
#include "../../render/draw/draw_commands.hpp"
#include "../../render/draw/draw_queue.hpp"

namespace base::menu::ui::notification {
  RgbColor Notification::GetNotificationColor(const Type type) {
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

  std::uint8_t Notification::GetAlpha(std::size_t creation_time_ms, std::size_t duration_ms) const {
    // Calculate time since creation in milliseconds
    const auto current_time = common::util::time::GetTimeStamp();
    const std::size_t elapsed_ms = current_time - creation_time_ms;

    // Fade in phase
    if (elapsed_ms < fade_in_duration_ms_) {
      return static_cast<std::uint8_t>((static_cast<float>(elapsed_ms) / fade_in_duration_ms_) * 255.f);
    }

    // Hold phase
    const auto total_active_time = fade_in_duration_ms_ + duration_ms;
    if (elapsed_ms < total_active_time) {
      return 255;
    }

    // Fade out phase
    const auto fade_out_start = total_active_time;
    const auto elapsed_fade_out = elapsed_ms - fade_out_start;
    if (elapsed_fade_out < fade_out_duration_ms_) {
      return static_cast<std::uint8_t>(255.f - (static_cast<float>(elapsed_fade_out) / fade_out_duration_ms_) * 255.f);
    }

    return 0;
  }

  float Notification::GetYOffset() const {
    return current_y_offset_;
  }


  std::float_t Notification::Draw(render::DrawQueueBuffer* draw_queue_buffer, const bool right_align, std::float_t y_offset, std::size_t creation_time_ms, std::size_t duration_ms) {
    if (!draw_queue_buffer) {
      return 0.f;
    }

    current_y_offset_ = y_offset;
    const std::uint8_t alpha = GetAlpha(creation_time_ms, duration_ms);

    // Calculate notification dimensions
    const ImVec2 notification_pos = {
      right_align ? 1.0f - notification_width_ : x_margin_,
      y_offset
    };

    // Calculate dynamic height based on content
    const float max_text_width = notification_width_ - (x_margin_ * 2);

    // Title height (1 line)
    ImVec2 title_size = render::draw_helpers::CalcTextSize(nullptr, title_text_size_, title_, max_text_width);

    // Message height (max 2 lines with wrapping)
    std::string wrapped_message = message_;
    if (!message_.empty()) {
      render::draw_helpers::WordWrap(message_text_size_, wrapped_message, max_text_width, 3);
    }
    ImVec2 message_size = render::draw_helpers::CalcTextSize(nullptr, message_text_size_, wrapped_message, max_text_width);

    // Calculate total notification height
    float notification_height = y_margin_;  // top margin
    notification_height += title_size.y;    // title height
    if (!message_.empty()) {
      notification_height += text_margin_;  // gap between title and message
      notification_height += message_size.y; // message height
    }
    notification_height += y_margin_;       // bottom margin

    const ImVec2 notification_size = {notification_width_, notification_height};

    // Apply alpha to colors
    const ImU32 background_color = RgbColor(30, 30, 30, alpha);
    const ImU32 top_bar_color = notification_color_;
    const ImU32 text_color = RgbColor(255, 255, 255, alpha);

    // Draw background rectangle
    draw_queue_buffer->AddCommand(render::RectBorder(
      notification_pos,
      notification_size,
      background_color,
      top_bar_color,
      false,  // top border
      false, // bottom border
      true, // left border
      false,  // right border
      top_bar_thickness_
    ));

    // Draw title text
    const ImVec2 title_pos = {
      notification_pos.x + x_margin_,
      notification_pos.y + y_margin_
    };
    draw_queue_buffer->AddCommand(render::PushFont(kMENU_RENDERER->GetTheme()->text_props.font_bold));
    draw_queue_buffer->AddCommand(render::Text(
      title_pos,
      text_color,
      title_,
      title_text_size_,
      false,
      false,
      false,
      max_text_width
    ));
    draw_queue_buffer->AddCommand(render::PopFont());

    // Draw message text (if not empty)
    if (!message_.empty()) {
      const ImVec2 message_pos = {
        notification_pos.x + x_margin_,
        notification_pos.y + y_margin_ + title_size.y + text_margin_
      };
      draw_queue_buffer->AddCommand(render::Text(
        message_pos,
        text_color,
        wrapped_message,
        message_text_size_,
        false,
        false,
        false,
        max_text_width,
        2  // max 2 lines
      ));
    }

    // Return the total height occupied by this notification including spacing
    return notification_height + y_margin_;
  }
}
