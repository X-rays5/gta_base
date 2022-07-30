//
// Created by X-ray on 06/28/22.
//

#include "notification.hpp"

#include <utility>

namespace ui {
  float Notification::NotificationData::Draw(const ImVec2& pos) {
    ImU32 color = GetColor();
    auto draw_list = kMANAGER->GetDrawList();
    auto font = ImGui::GetFont();
    auto title_tmp = title_;
    auto body_tmp = body_;

    util::draw::WordWrap(font_size_title, title_tmp, x_size, 1);

    auto y_size_char_title = util::draw::CalcTextSize(font, font_size_title, " ").y;
    auto y_size_char_body = util::draw::CalcTextSize(font, font_size_body, " ").y;
    float y_textbox_size = y_size;
    if (!title_tmp.empty())
      y_textbox_size += y_size_char_title;
    if (util::draw::WordWrap(font_size_body, body_tmp, x_size, 3) == 1) {
      body_tmp += "\n "; // fix positioning issues without multiline
      y_textbox_size += y_size_char_body;
    } else {
      y_textbox_size += util::draw::CalcTextSize(font, font_size_body, body_tmp).y;
    }
    auto y_pos_body = pos.y;
    if (!title_tmp.empty())
      y_pos_body += (y_size_char_title - (y_size_char_title / 5.f));
    else
      y_pos_body -= y_size_char_body / 2.f;

    draw_list->AddCommand(util::draw::Rect(pos, {x_size, y_top_bar_size}, color));
    draw_list->AddCommand(util::draw::Rect({pos.x, pos.y + y_top_bar_size}, {x_size, y_textbox_size}, ImColor(0,0,0,255)));

    draw_list->AddCommand(util::draw::Text({pos.x + 0.002f, pos.y}, text_color, title_tmp, false, true, font_size_title));
    draw_list->AddCommand(util::draw::Text({pos.x + 0.002f, y_pos_body}, text_color, body_tmp, false, true, font_size_body));

    return y_textbox_size;
  }

  void Notification::Create(Type type, std::string title, std::string description, std::uint32_t duration) {
    mtx_.lock();
    notifications_.emplace_back(NotificationData(type, std::move(title), std::move(description), duration));
    mtx_.unlock();
  }

  void Notification::Tick() {
    float y_pos = y_base;
    mtx_.lock();
    for (int i = 0; i < notifications_.size(); i++) {
      auto notification = &notifications_[i];

      if ((util::UnixEpoch() - notification->start_time_) > notification->duration_) {
        notifications_.erase(notifications_.begin() + i);
      } else {
        auto increase = notification->Draw(ImVec2(x_base, y_pos)) + y_spacing + y_top_bar_size;
        y_pos += increase;

        // screen is full don't overdraw
        if (y_pos + increase > 1.f) {
          break;
        }
      }
    }
    mtx_.unlock();
  }
}
