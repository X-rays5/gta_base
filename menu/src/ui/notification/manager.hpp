//
// Created by X-ray on 04/05/2024.
//

#ifndef MANAGER_HPP_05174239
#define MANAGER_HPP_05174239
#include <vector>
#include "notification.hpp"
#include "../../scripts/base_script.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui::notification {
  class Manager final : public scripts::BaseScript {
  public:
    struct Notify {
      std::size_t time_start;
      std::size_t duration_ms;
      Notification notification;
    };

  public:
    Manager();
    virtual ~Manager() override;

    virtual void ScriptInit() override {}
    virtual void ScriptTick() override;

    virtual Type GetScriptType() const override {
      return Type::MenuRenderThread;
    }

    /**
   * Display a notification for a specified amount of time
   * @param type Type of notification to display: Info | Warning | Error
   * @param duration_ms Duration to display the notification for in ms
   * @param title The title to display the notification with
   * @param message The message to display the notification with
   */
    void AddNotification(NotificationType type, std::size_t duration_ms, const std::string& title, const std::string& message);

  private:
    std::vector<Notify> notifications_;
    std::size_t script_id_ = 0;
  };

  inline Manager* kMANAGER{};
}

#define ADD_NOTIFICATION(type, title, message) ::base::ui::notification::kMANAGER->AddNotification(type, 5000, title, message)
#define NOTIFY_INFO(title, message) ADD_NOTIFICATION(::base::ui::notification::Type::Info, title, message); LOG_INFO("[NOTIFY] {}: {}", title, message)
#define NOTIFY_WARN(title, message) ADD_NOTIFICATION(::base::ui::notification::Type::Warning, title, message); LOG_WARN("[NOTIFY] {}: {}", title, message)
#define NOTIFY_ERR(title, message) ADD_NOTIFICATION(::base::ui::notification::Type::Error, title, message); LOG_ERROR("[NOTIFY] {}: {}", title, message)

#endif //MANAGER_HPP_05174239
