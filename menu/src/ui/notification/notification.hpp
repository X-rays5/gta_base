//
// Created by X-ray on 04/05/2024.
//

#ifndef NOTIFICATION_HPP_05174124
#define NOTIFICATION_HPP_05174124
#include <imgui/imgui.h>
#include "../../render/animate.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}


namespace base::menu::ui::notification {
  class Manager;

  enum class NotificationType {
    Info,
    Warning,
    Error
  };

  class Notification {
  private:
    Notification(const NotificationType type, const std::string& title, const std::string& message) : type_(type), title_(title), message_(message), notification_color_(GetNotificationColor(type)) {}

    static ImColor GetNotificationColor(NotificationType type);

    [[nodiscard]] std::float_t Draw(menu::render::DrawQueueBuffer* draw_queue_buffer, bool right_align, std::float_t y_offset) const;

    friend class Manager;

  private:
    NotificationType type_;
    std::string title_;
    std::string message_;
    //std::unique_ptr<render::animate::LerpWaitReturn<std::float_t>> notification_animation_;
    ImColor notification_color_;

    static constexpr auto slide_duration_ms_ = 500;
    static constexpr auto top_bar_thickness_ = 0.003f;
    static constexpr auto notification_transparency_ = 213;
    static constexpr auto notification_width_ = 0.10f;
    static constexpr auto title_text_size_ = 0.018f;
    static constexpr auto x_margin_ = 0.006f;
    static constexpr auto y_margin_ = 0.01f;
    static constexpr auto text_margin_ = 0.002f;

    static constexpr auto info_color_ = ImColor(255, 255, 255);
    static constexpr auto warning_color_ = ImColor(255, 165, 0);
    static constexpr auto error_color_ = ImColor(255, 0, 0);
  };
};

#endif //NOTIFICATION_HPP_05174124
