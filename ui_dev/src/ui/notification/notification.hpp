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
#include "imgui.h"
#include "../manager.hpp"
#include "../util/util.hpp"

namespace ui {
  class Notification {
  public:
    enum class Type {
      kSuccess,
      kFail,
      kInfo
    };

    struct NotificationData {
    public:
      NotificationData(Type type, std::string title, std::string body, std::uint64_t duration = 2000) : type_(type), title_(std::move(title)), body_(std::move(body)), duration_(duration) {}

      void Draw(const ImVec2& pos);

      Type type_;
      std::string title_;
      std::string body_;
      std::uint64_t duration_;
      std::uint64_t start_time_ = util::UnixEpoch();

    private:
      [[nodiscard]] inline ImU32 GetColor() const {
        switch (type_) {
          case Type::kSuccess:
            return success_color;
          case Type::kFail:
            return fail_color;
          case Type::kInfo:
            return info_color;
        }

        return {};
      }

      constexpr static const ImU32 success_color = 0x00ff00ff;
      constexpr static const ImU32 fail_color = 0xff0000ff;
      constexpr static const ImU32 info_color = 0xffffffff;
    };

    void Create();

    void Tick();

  private:
    constexpr static const float x_size = 0.15f;
    constexpr static const float x_base = 1.f - x_size;
    constexpr static const float y_base = 0.015f;
    constexpr static const float y_size = 0.08f;
    constexpr static const float y_spacing = y_base;
    constexpr static const float y_top_bar_size = 0.008f;
    constexpr static const float font_size_title = 0.025f;
    constexpr static const float font_size_body = 0.018f;
    constexpr static const ImU32 text_color = 0xffffffff;

    std::vector<NotificationData> notifications_;
  };

  inline std::unique_ptr<Notification> kNOTIFICATIONS;
}
#endif //GTA_BASE_NOTIFICATION_HPP
