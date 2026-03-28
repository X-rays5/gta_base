//
// Created by X-ray on 04/05/2024.
//

#ifndef MANAGER_HPP_05174239
#define MANAGER_HPP_05174239
#include <vector>
#include "notification.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::ui::notification {
  class Manager {
  public:
    struct Notify {
      std::size_t time_start;
      std::size_t duration_ms;
      Notification notification;
    };

  public:
    Manager();
    ~Manager();

    /**
   * Display a notification for a specified amount of time
   * @param type Type of notification to display: Info | Warning | Error
   * @param duration_ms Duration to display the notification for in ms
   * @param title The title to display the notification with
   * @param message The message to display the notification with
   */
    void AddNotification(const Type type, const std::size_t duration_ms, const std::string& title, const std::string& message);

    void Render(menu::render::DrawQueueBuffer* draw_queue_buffer);

  private:
    std::vector<Notify> notifications_;
  };

  inline Manager* kMANAGER{};
}

#define ADD_NOTIFICATION(type, title, ...) do { if (::base::ui::notification::kMANAGER ) { ::base::ui::notification::kMANAGER->AddNotification(type, 5000, title, ""); } } while (false)
#define NOTIFY_INFO(title, ...) ADD_NOTIFICATION(::base::ui::notification::Type::Info, title, message); LOG_INFO("[NOTIFY] {}", title)
#define NOTIFY_WARN(title, ...) ADD_NOTIFICATION(::base::ui::notification::Type::Warning, title, message); LOG_WARN("[NOTIFY] {}", title)
#define NOTIFY_ERR(title, ...) ADD_NOTIFICATION(::base::ui::notification::Type::Error, title, message); LOG_ERROR("[NOTIFY] {}", title)

#endif //MANAGER_HPP_05174239
