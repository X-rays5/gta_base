//
// Created by X-ray on 04/05/2024.
//

#include "notification.hpp"

#include <imgui/imgui.h>

#include <imfont/IconsFontAwesome6.hpp>
#include "../../memory/pointers.hpp"
#include "../../render/draw.hpp"
#include "../../render/draw_util.hpp"

namespace base::ui::notification {
  namespace {
    std::string GetTypeText(const Type type) {
      std::string type_text;

      switch (type) {
      case Type::Info:
        type_text = ICON_FA_CIRCLE_INFO "Info";
        break;
      case Type::Warning:
        type_text = ICON_FA_TRIANGLE_EXCLAMATION "Warning";
        break;
      case Type::Error:
        type_text = ICON_FA_CIRCLE_XMARK "Error";
        break;
      default:
        type_text = "Unknown";
      }

      return type_text;
    }
  }

  void Notification::Draw(menu::render::DrawQueueBuffer* draw_queue_buffer, const std::size_t delta_time, const bool right_align, const std::float_t y_offset, const std::size_t duration_ms) {
    const std::string type_text = GetTypeText(type_);

    const auto notification_x_pos = right_align ? 1.f - notification_width_ : 0;

    if (!notification_animation_) {
      notification_animation_ = std::make_unique<render::animate::LerpWaitReturn<std::float_t>>(right_align ? 1.f : 0.f, notification_x_pos, duration_ms / 10, duration_ms - (duration_ms / 10));
    }

    auto x = notification_animation_->Animate(delta_time);
    draw_queue_buffer->AddCommand(menu::render::Text({x, y_offset}, ImColor(255, 0, 0), "Hello World2!", 0.02f));
  }
}
