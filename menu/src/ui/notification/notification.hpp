//
// Created by X-ray on 04/05/2024.
//

#ifndef NOTIFICATION_HPP_05174124
#define NOTIFICATION_HPP_05174124
#include <imgui/imgui.h>

#include "../theme.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}


namespace base::menu::ui::notification {
  class Manager;

  enum class Type {
    Info,
    Warning,
    Error
  };

  class Notification {
  private:
    Notification(const Type type, const std::string& title, const std::string& message)
      : type_(type), title_(title), message_(message),
        notification_color_(GetNotificationColor(type)) {}

    static RgbColor GetNotificationColor(Type type);

    [[nodiscard]] std::float_t Draw(render::DrawQueueBuffer* draw_queue_buffer, bool right_align, std::float_t y_offset, std::size_t creation_time_ms, std::size_t duration_ms);

    [[nodiscard]] std::uint8_t GetAlpha(std::size_t creation_time_ms, std::size_t duration_ms) const;
    [[nodiscard]] float GetYOffset() const;

    friend class Manager;

  private:
    Type type_;
    std::string title_;
    std::string message_;
    RgbColor notification_color_;

    float current_y_offset_{0.f};

    static constexpr auto slide_duration_ms_ = 500;
    static constexpr auto fade_in_duration_ms_ = 300;
    static constexpr auto fade_out_duration_ms_ = 300;
    static constexpr auto top_bar_thickness_ = 0.003f;
    static constexpr auto notification_transparency_ = 255;
    static constexpr auto notification_width_ = 0.12f;
    static constexpr auto notification_height_ = 0.08f;
    static constexpr auto title_text_size_ = 0.013f;
    static constexpr auto message_text_size_ = 0.01f;
    static constexpr auto x_margin_ = 0.006f;
    static constexpr auto y_margin_ = 0.005f;
    static constexpr auto text_margin_ = 0.002f;

    // Calculate max visible notifications based on screen height
    // Available height = 1.0f, each notification takes: notification_height + y_margin
    // Max visible = floor(1.0f / (notification_height + y_margin))
    static constexpr auto GetMaxVisibleNotifications() {
      return static_cast<std::size_t>(1.0f / (notification_height_ + y_margin_));
    }

    static constexpr auto info_color_ = RgbColor(74, 144, 226);
    static constexpr auto warning_color_ = RgbColor(255, 165, 0);
    static constexpr auto error_color_ = RgbColor(255, 0, 0);
  };
};

#endif //NOTIFICATION_HPP_05174124
