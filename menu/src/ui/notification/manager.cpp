//
// Created by X-ray on 04/05/2024.
//

#include "manager.hpp"
#include "../../render/thread.hpp"
#include "../../render/renderer.hpp"

namespace base::ui::notification {
  Manager::Manager() {
    kMANAGER = this;
    menu::render::kTHREAD->AddRenderCallback(999, [](menu::render::DrawQueueBuffer* draw_queue_buffer) {
      if (kMANAGER) {
        kMANAGER->Render(draw_queue_buffer);
      }
    });
  }

  Manager::~Manager() {
    kMANAGER = nullptr;
  }

  void Manager::AddNotification(const Type type, const std::size_t duration_ms, const std::string& title, const std::string& message) {
    switch (type) {
    case Type::Info:
      LOG_INFO("Notification: \n{}\n{}", title, message);
      break;
    case Type::Warning:
      LOG_WARN("Notification: \n{}\n{}", title, message);
      break;
    case Type::Error:
      LOG_ERROR("Notification: \n{}\n{}", title, message);
      break;
    default:
      LOG_ERROR("Unknown notification type'{}': \n{}\n{}", magic_enum::enum_name(type), title, message);
      return;
    }

    notifications_.emplace_back(Notify{duration_ms, Notification(type, title, message)});
  }

  void Manager::Render(menu::render::DrawQueueBuffer* draw_queue_buffer) {
    // Iterate through all notifications and render them also remove them if they are expired
    for (auto it = notifications_.begin(); it != notifications_.end();) {
      it->notification.Draw(draw_queue_buffer, menu::render::kRENDERER->GetDeltaTime(), true, 0.0f, it->duration_ms);
      if (it->duration_ms == 0) {
        LOG_DEBUG("Notification expired: {}", it->notification.title_);
        it = notifications_.erase(it);
      } else {
        ++it;
      }
    }
  }
}
