//
// Created by X-ray on 04/05/2024.
//

#include "manager.hpp"
#include "../../render/render_thread.hpp"
#include "../../render/renderer.hpp"
#include <base-common/util/time.hpp>

namespace base::menu::ui::notification {
  Manager::Manager() {
    kNOTIFICATION_MANAGER = this;
    menu::render::kRENDER_THREAD->AddRenderCallback(999, [](menu::render::DrawQueueBuffer* draw_queue_buffer) {
      if (kNOTIFICATION_MANAGER) {
        kNOTIFICATION_MANAGER->Render(draw_queue_buffer);
      }
    });
  }

  Manager::~Manager() {
    kNOTIFICATION_MANAGER = nullptr;
  }

  void Manager::Render(render::DrawQueueBuffer* draw_queue_buffer) {
    if (!draw_queue_buffer) {
      return;
    }

    common::concurrency::ScopedSpinlock lock(notification_lock_);

    const auto current_time = common::util::time::GetTimeStamp();

    // Remove expired notifications
    std::erase_if(notifications_,
                  [current_time](const Notify& notif) {
                    const auto elapsed_ms = current_time - notif.time_start;
                    // Remove only after fade-in + hold + fade-out completes
                    return elapsed_ms >= (notif.duration_ms + Notification::fade_in_duration_ms_ + Notification::fade_out_duration_ms_);
                  });

    // Calculate max visible based on screen height and notification dimensions
    constexpr std::size_t max_visible = Notification::GetMaxVisibleNotifications();
    const std::size_t notifications_to_render = std::min(notifications_.size(), max_visible);

    // Render visible notifications from top to bottom (right aligned)
    float current_y_offset = Notification::y_margin_;  // Start from top

    for (std::size_t i = 0; i < notifications_to_render; ++i) {
      auto& notif = notifications_[i];

      // Draw the notification and get its height
      const float notification_height = notif.notification.Draw(draw_queue_buffer, true, current_y_offset, notif.time_start, notif.duration_ms);

      if (notification_height > 0.f) {
        current_y_offset += notification_height;
      }
    }
  }
}