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
#include "imgui.h"

namespace gta_base::ui {
class Notification {
 public:
  enum class Type {
    kSuccess,
    kFail,
    kInfo
  };

  struct NotificationData {
   public:
    NotificationData(Type type, std::string title, std::string body, std::uint64_t duration) : type_(type), title_(std::move(title)), body_(std::move(body)), duration_(duration) {}

    float Draw(const ImVec2 &pos);

    Type type_;
    std::string title_;
    std::string body_;
    std::uint64_t duration_;
    std::uint64_t start_time_ = common::GetEpoch();

   private:
    [[nodiscard]] inline ImU32 GetColor() const {
      switch (type_) {
        case Type::kSuccess:return success_color;
        case Type::kFail:return fail_color;
        case Type::kInfo:return info_color;
      }

      return {};
    }

    static constexpr const auto success_color = ImColor(0.f, 255.f, 0.f);
    static constexpr const auto fail_color = ImColor(255.f, 0.f, 0.f);
    static constexpr const auto info_color = ImColor(255.f, 255.f, 255.f);
  };

  Notification();
  ~Notification();

  void Create(Type type, std::string title, std::string description, std::uint32_t duration = 3000);

  void Tick();

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
  std::mutex mtx_;
};

inline Notification *kNOTIFICATIONS{};
}
#endif //GTA_BASE_NOTIFICATION_HPP