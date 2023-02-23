//
// Created by X-ray on 06/28/22.
//

#ifndef GTA_BASE_NOTIFICATION_HPP
#define GTA_BASE_NOTIFICATION_HPP
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <ctime>
#include <mutex>
#include <imgui.h>
#include "../../d3d/draw.hpp"

namespace gta_base::ui {
  class NotificationManager;

  inline NotificationManager* kNOTIFICATIONS{};

  enum class NotificationType {
    kSuccess,
    kFail,
    kInfo
  };

  class NotificationManager {
  public:
    struct NotificationData {
    public:
      NotificationData(NotificationType type, std::string title, std::string body, std::uint64_t duration) : type_(type), title_(std::move(title)), body_(std::move(body)), duration_(duration), draw_list_(kNOTIFICATIONS->GetDrawList()) {}

      float Draw(const ImVec2& pos);

      NotificationType type_;
      std::string title_;
      std::string body_;
      std::uint64_t duration_;
      std::uint64_t start_time_ = common::GetEpoch();
      d3d::draw::DrawList* draw_list_;

    private:
      [[nodiscard]] inline ImU32 GetColor() const {
        switch (type_) {
          case NotificationType::kSuccess:
            return success_color;
          case NotificationType::kFail:
            return fail_color;
          case NotificationType::kInfo:
            return info_color;
        }

        return {};
      }

      static constexpr const auto success_color = ImColor(0.f, 255.f, 0.f);
      static constexpr const auto fail_color = ImColor(255.f, 0.f, 0.f);
      static constexpr const auto info_color = ImColor(255.f, 255.f, 255.f);
    };

  public:
    explicit NotificationManager(d3d::draw::DrawList* draw_list);
    ~NotificationManager();

    void Create(NotificationType type, std::string title, std::string description, std::uint32_t duration = 3000);

    void Tick();

  protected:
    FORCE_INLINE d3d::draw::DrawList* GetDrawList() {
      return draw_list_;
    }

  private:
    constexpr static const float x_size = 0.16f;
    constexpr static const float x_base_left = 0.0f;
    constexpr static const float x_base_right = 1.f - x_size;
    constexpr static const float y_base = 0.015f;
    constexpr static const float y_size = 0.01f;
    constexpr static const float y_spacing = 0.01f;
    constexpr static const float y_top_bar_size = 0.004f;
    constexpr static const float font_size_title = 0.022f;
    constexpr static const float font_size_body = 0.018f;
    constexpr static const ImU32 text_color = 0xffffffff;

    std::vector<NotificationData> notifications_;
    d3d::draw::DrawList* draw_list_;
    std::mutex mtx_;
  };
}
#endif //GTA_BASE_NOTIFICATION_HPP