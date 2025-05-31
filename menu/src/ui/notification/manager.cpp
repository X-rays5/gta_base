//
// Created by X-ray on 04/05/2024.
//

#include "manager.hpp"
#include "../../render/render_thread.hpp"
#include "../../render/renderer.hpp"

namespace base::ui::notification {
  Manager::Manager() {
    kMANAGER = this;
    menu::render::kRENDER_THREAD->AddRenderCallback(999, [](menu::render::DrawQueueBuffer* draw_queue_buffer) {
      if (kMANAGER) {
        kMANAGER->Render(draw_queue_buffer);
      }
    });
  }

  Manager::~Manager() {
    kMANAGER = nullptr;
  }

  void Manager::AddNotification(const Type type, const std::size_t duration_ms, const std::string& title, const std::string& message) {
    Notify notif = {common::util::time::GetTimeStamp(), duration_ms, Notification(type, title, message)};
    notifications_.emplace_back(notif);
  }

  void Manager::Render(menu::render::DrawQueueBuffer* draw_queue_buffer) {
    // Iterate through all notifications and render them also remove them if they are expired
    std::float_t y_offset = 0.0f;
    for (auto it = notifications_.begin(); it != notifications_.end();) {
      y_offset += it->notification.Draw(draw_queue_buffer, true, y_offset);
      if (common::util::time::GetTimeStamp() - it->time_start >= it->duration_ms) {
        LOG_DEBUG("Notification expired: {}", it->notification.title_);
        it = notifications_.erase(it);
      } else {
        ++it;
      }
    }
  }
}
