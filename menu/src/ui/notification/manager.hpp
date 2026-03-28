//
// Created by X-ray on 04/05/2024.
//

#ifndef MANAGER_HPP_05174239
#define MANAGER_HPP_05174239
#include <vector>
#include <base-common/concurrency/spinlock.hpp>
#include "notification.hpp"
#include "../localization/manager.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui::notification {
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
   * @param message_args The message to display the notification with
   */
    template <typename... Args>
void AddNotification(const Type type,
                     const std::size_t duration_ms,
                     const std::string& title,
                     Args&&... message_args)
    {
      std::string message;

      if constexpr (sizeof...(message_args) > 0) {
        auto args_tuple = std::make_tuple(std::forward<Args>(message_args)...);

        const auto& fmt_str =
            localization::kMANAGER->Localize(std::get<0>(args_tuple));

        message = std::apply(
            [&](const auto& first, const auto&... rest) {
                if constexpr (sizeof...(rest) > 0) {
                    return fmt::vformat(
                        fmt_str,
                        fmt::make_format_args(rest...)
                    );
                } else {
                    return fmt_str;
                }
            },
            args_tuple
        );
      } else {
        message.clear();
      }

      common::concurrency::ScopedSpinlock lock(notification_lock_);
      Notify notif = {
        common::util::time::GetTimeStamp(),
        duration_ms,
        Notification(
            type,
            localization::kMANAGER->Localize(title),
            message
        )
    };
      notifications_.emplace_back(notif);
    }

    void Render(render::DrawQueueBuffer* draw_queue_buffer);

  private:
    common::concurrency::Spinlock notification_lock_;
    std::vector<Notify> notifications_;
    static constexpr std::size_t max_queued_notifications_ = 100; // Allow queuing beyond visible ones
  };

  inline Manager* kNOTIFICATION_MANAGER{};
}

#define ADD_NOTIFICATION(type, title, ...)                                                         \
    do {                                                                                           \
        if (::base::menu::ui::notification::kNOTIFICATION_MANAGER) {                               \
            ::base::menu::ui::notification::kNOTIFICATION_MANAGER->AddNotification(                \
                type, 5000, title, ##__VA_ARGS__);                                                  \
        }                                                                                          \
    } while (false)

#define NOTIFY_INFO(title, ...)                                                                    \
    do {                                                                                           \
        std::string _msg = fmt::format(__VA_ARGS__);                                                \
        ADD_NOTIFICATION(::base::menu::ui::notification::Type::Info, title, ##__VA_ARGS__);                 \
        LOG_INFO("[NOTIFY] {} - {}", title, _msg);                                                  \
    } while (false)

#define NOTIFY_WARN(title, ...)                                                                    \
    do {                                                                                           \
        std::string _msg = fmt::format(__VA_ARGS__);                                                \
        ADD_NOTIFICATION(::base::menu::ui::notification::Type::Warning, title, ##__VA_ARGS__);              \
        LOG_WARN("[NOTIFY] {} - {}", title, _msg);                                                  \
    } while (false)

#define NOTIFY_ERR(title, ...)                                                                     \
    do {                                                                                           \
        std::string _msg = fmt::format(__VA_ARGS__);                                                \
        ADD_NOTIFICATION(::base::menu::ui::notification::Type::Error, title, ##__VA_ARGS__);                \
        LOG_ERROR("[NOTIFY] {} - {}", title, _msg);                                                 \
    } while (false)

#endif //MANAGER_HPP_05174239
