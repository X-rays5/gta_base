//
// Created by X-ray on 06/28/22.
//

#include "notification.hpp"
#include "../manager.hpp"
#include "../../d3d/renderer.hpp"

namespace gta_base::ui {
  NotificationManager::NotificationManager(d3d::draw::DrawList* draw_list) : draw_list_(draw_list) {
    kNOTIFICATIONS = this;
  }

  NotificationManager::~NotificationManager() {
    kNOTIFICATIONS = nullptr;
  }

  float NotificationManager::NotificationData::Draw(const ImVec2& pos) {
    ImU32 color = GetColor();
    auto font = d3d::kRENDERER->GetFont();
    auto font_bold = d3d::kRENDERER->GetFontBold();
    auto title_tmp = title_;
    auto body_tmp = body_;

    //ImGui::PushFont(font_bold);
    d3d::draw::WordWrap(font_size_title, title_tmp, x_size + 0.02f, 1);
    //ImGui::PopFont();

    auto y_size_char_title = d3d::draw::CalcTextSize(font_bold, font_size_title, " ").y;
    auto y_size_char_body = d3d::draw::CalcTextSize(font, font_size_body, " ").y;
    float y_textbox_size = y_size;
    if (!title_tmp.empty())
      y_textbox_size += y_size_char_title;

    d3d::draw::WordWrap(font_size_body, body_tmp, x_size + 0.005f, 3);
    y_textbox_size += d3d::draw::CalcTextSize(font, font_size_body, body_tmp).y;

    auto y_pos_body = pos.y;
    if (!title_tmp.empty()) {
      y_pos_body += (y_size_char_title - (y_size_char_title / 5.f)) + (y_size_char_title / 2.f);
    } else {
      y_pos_body -= y_size_char_body - 0.025f;
    }

    if (body_tmp.empty()) {
      y_textbox_size -= y_size_char_body * 1.25f;
    }

    draw_list_->AddCommand(d3d::draw::Rect(pos, {x_size, y_top_bar_size}, color));
    draw_list_->AddCommand(d3d::draw::Rect({pos.x, pos.y + y_top_bar_size}, {x_size, y_textbox_size}, ImColor(0, 0, 0, 255)));

    draw_list_->AddCommand(d3d::draw::Text({pos.x + 0.002f, pos.y + 0.005f}, text_color, title_tmp, false, false, font_size_title, font_bold));
    draw_list_->AddCommand(d3d::draw::Text({pos.x + 0.002f, y_pos_body}, text_color, body_tmp, false, false, font_size_body));

    return y_textbox_size;
  }

  void NotificationManager::Create(NotificationType type, std::string title, std::string description, std::uint32_t duration) {
    LOG_INFO("[NOTIFICATION]: \nType: {}\nTitle: {}\nBody: {}", magic_enum::enum_name(type), title, description);
    mtx_.lock();
    notifications_.emplace_back(type, std::move(title), std::move(description), duration);
    mtx_.unlock();
  }

  void NotificationManager::Tick() {
    float x_base;
    if (kUI_MANAGER->GetUI()->GetTheme()->base_x <= 0.5f)
      x_base = x_base_right;
    else
      x_base = x_base_left;

    float y_pos = y_base;
    mtx_.lock();
    for (int i = 0; i < notifications_.size(); i++) {
      auto notification = &notifications_[i];

      if ((common::GetEpoch() - notification->start_time_) > notification->duration_) {
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