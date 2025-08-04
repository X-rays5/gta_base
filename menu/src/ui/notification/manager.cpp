//
// Created by X-ray on 04/05/2024.
//

#include "manager.hpp"
#include "../../render/renderer.hpp"
#include "../../scripts/script_manager.hpp"

namespace base::menu::ui::notification {
  Manager::Manager() {
    script_id_ = scripts::kSCRIPTMANAGER->AddScript(this);

    kMANAGER = this;
  }

  Manager::~Manager() {
    kMANAGER = nullptr;

    scripts::kSCRIPTMANAGER->RemoveScript(script_id_, GetScriptType());
  }

  void Manager::ScriptTick() {
    const auto draw_queue_buffer = render::kRENDERER->GetDrawQueueBuffer();

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

  void Manager::AddNotification(const NotificationType type, const std::size_t duration_ms, const std::string& title, const std::string& message) {
    Notify notif = {common::util::time::GetTimeStamp(), duration_ms, Notification(type, title, message)};
    notifications_.emplace_back(notif);
  }
}
