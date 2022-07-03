//
// Created by X-ray on 06/28/22.
//

#include "notification.hpp"

namespace ui {
  void Notification::NotificationData::Draw(const ImVec2& pos) {
    ImU32 color = GetColor();
    auto draw_list = kMANAGER->GetDrawList();

    draw_list->AddCommand(util::draw::Rect(pos, {x_size, y_top_bar_size}, color));
    draw_list->AddCommand(util::draw::Rect({pos.x, pos.y + y_top_bar_size}, {x_size, y_size}, ImColor(0,0,0,255)));

    draw_list->AddCommand(util::draw::Text({pos.x + 0.002f, pos.y}, text_color, title_, false, true, font_size_title));
    draw_list->AddCommand(util::draw::Text({pos.x + 0.002f, pos.y + 0.02f}, text_color, title_, false, true, font_size_body));
  }

  void Notification::Create() {
    notifications_.emplace_back(NotificationData(Type::kInfo, "epic title", "This is a story about nothing and\n it's actually just a word wrap test", 3000));
    notifications_.emplace_back(NotificationData(Type::kInfo, "test", "bigger test", 5000));
  }

  void Notification::Tick() {
    float y_pos = y_base;
    for (int i = 0; i < notifications_.size(); i++) {
      auto notification = &notifications_[i];

      if ((util::UnixEpoch() - notification->start_time_) > notification->duration_) {
        notifications_.erase(notifications_.begin() + i);
      } else {
        notification->Draw(ImVec2(x_base, y_pos));

        y_pos += y_size + y_spacing + y_top_bar_size;
      }
    }
  }
}
