//
// Created by X-ray on 3/9/2022.
//

#include "manager.hpp"
#include <sstream>
#include "notification/notification.hpp"

namespace ui {
  inline float ScaleFps(float number) {
    return number * (144.f / ImGui::GetIO().Framerate);
  }

  Manager::Manager() {
    draw_list_ = std::make_shared<util::draw::DrawList>(2);
    input_left_ = std::make_unique<util::TimedInput>(VK_LEFT, 140);
    input_right_ = std::make_unique<util::TimedInput>(VK_RIGHT, 140);
    input_up_ = std::make_unique<util::TimedInput>(VK_UP, 100);
    input_down_ = std::make_unique<util::TimedInput>(VK_DOWN, 100);
    input_return_ = std::make_unique<util::TimedInput>(VK_RETURN, 300);
    input_back_ = std::make_unique<util::TimedInput>(VK_BACK, 300);

    kNOTIFICATIONS = std::make_unique<Notification>();
  }

  __forceinline util::draw::Text Manager::DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center) const {
    return {{x_base + 0.002f, y_pos}, color, text, false, center, font_size};
  }

  __forceinline util::draw::Text Manager::DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center) const {
    return {{x_base + x_size - 0.002f, y_pos}, color, text, true, center, font_size};
  }

  __forceinline void Manager::DrawHeader() {

  }

  __forceinline void Manager::DrawTopBar(const std::string& title, size_t option_current, size_t option_count) {
    draw_list_->AddCommand(util::draw::Rect({x_base, y_base}, {x_size, y_size_top_bar}, primary_color.load()));
    draw_list_->AddCommand(DrawTextLeft(y_base - (y_size_top_bar / 4), text_color.load(), title));

    std::stringstream option_count_str;
    option_count_str << option_current + 1 << '/' << option_count;
    draw_list_->AddCommand(DrawTextRight(y_base - (y_size_top_bar / 4), text_color.load(), option_count_str.str()));
  }

  __forceinline void Manager::DrawBottomBar(size_t option_count) {
    std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;

    float y_pos = (y_base + y_size_top_bar) + (y_size_option * visible_options);
    draw_list_->AddCommand(util::draw::Rect({x_base, y_pos}, {x_size, y_size_bottom_bar}, primary_color.load()));
    draw_list_->AddCommand(DrawTextRight(y_pos - (y_size_bottom_bar / 4), text_color.load(), "v1.0.0")); // TODO: replace this with an a real version string or something
  }

  __forceinline void Manager::DrawScrollBar(size_t option_count, int current_option) {
    std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;
    float scrollbar_y_area = (y_size_option * visible_options);

    draw_list_->AddCommand(util::draw::Rect({x_base + (x_size + scrollbar_offset), y_base + y_size_top_bar}, {x_size_scrollbar, scrollbar_y_area}, primary_color.load()));

    float scroller_y_size = scrollbar_y_area / option_count;
    DrawScrollBarScroller(y_base + (current_option * scroller_y_size) + y_size_top_bar, scroller_y_size);
  }

  __forceinline void Manager::DrawScrollBarScroller(float target_pos, float scroller_y_size) {
    if (scroller_current_pos_ == -1.f || scrollbar_reset_) {
      scrollbar_reset_ = false;
      scrollbar_current_pos_ = target_pos;
    }

    if (scrollbar_current_pos_ < target_pos) {
      scrollbar_current_pos_ += util::draw::ScaleYFromScreen(ScaleFps(scrollbar_speed_));
      if (scrollbar_current_pos_ > target_pos)
        scrollbar_reset_ = true;
    } else if (scrollbar_current_pos_ > target_pos) {
      scrollbar_current_pos_ -= util::draw::ScaleYFromScreen(ScaleFps(scrollbar_speed_));
      if (scrollbar_current_pos_ < target_pos)
        scrollbar_reset_ = true;
    }

    draw_list_->AddCommand(util::draw::Rect({x_base + (x_size + scrollbar_offset), scrollbar_current_pos_}, {x_size_scrollbar, scroller_y_size}, secondary_color.load()));
  }

  __forceinline void Manager::DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count, size_t option_idx) {
    float pos = y_base + (y_size_option * option_pos) + y_size_top_bar;
    float text_pos = pos - (y_size_option / 4);
    if (selected) {
      if (previous_selected_option_ == 0 && option_idx == (sub_option_count - 1)) {
        ResetSmoothScrolling();
      } else if (previous_selected_option_ == (sub_option_count - 1) && option_idx == 0) {
        ResetSmoothScrolling();
      }
      previous_selected_option_ = (int)option_idx;

      auto color = DrawScroller(pos) ? selected_text_color.load() : text_color.load();
      draw_list_->AddCommand(DrawTextLeft(text_pos , color, option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(text_pos, color, option->GetRightText()));
    } else {
      draw_list_->AddCommand(DrawTextLeft(text_pos, text_color.load(), option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(text_pos, text_color.load(), option->GetRightText()));
    }

    if (selected && !option->GetDescription().empty()) {
      DrawDescriptionText(option->GetDescription(), sub_option_count > max_drawn_options ? max_drawn_options : sub_option_count);
    }
  }

  __forceinline bool Manager::DrawScroller(float target_pos) {
    if (scroller_current_pos_ == -1.f || scroller_reset_){
      scroller_reset_ = false;
      scroller_current_pos_ = target_pos;
    }

    if (scroller_current_pos_ < target_pos) {
      scroller_current_pos_ += util::draw::ScaleYFromScreen(ScaleFps(scroller_speed_));
      if (scroller_current_pos_ > target_pos)
        scroller_reset_ = true;
    } else if (scroller_current_pos_ > target_pos) {
      scroller_current_pos_ -= util::draw::ScaleYFromScreen(ScaleFps(scroller_speed_));
      if (scroller_current_pos_ < target_pos)
        scroller_reset_ = true;
    }

    draw_list_->AddCommand(util::draw::Rect({x_base, scroller_current_pos_}, {x_size, y_size_option}, scroller_color.load()));

    return (scroller_current_pos_ > (target_pos - (y_size_option / 2.f)) && scroller_current_pos_ < (target_pos + (y_size_option / 2.f)));
  }


  __forceinline void Manager::DrawDescriptionText(const std::string& description, size_t option_count) const {
    constexpr static const float y_size_separator = 0.0046f;
    float y_pos = (y_base + (y_size_option * option_count)) + (y_size_bottom_bar + y_offset_description) + y_size_top_bar;
    float y_pos_text_box = y_pos + y_size_separator;


    draw_list_->AddCommand(util::draw::Rect({x_base, y_pos}, {x_size, y_size_separator}, secondary_color.load()));
    draw_list_->AddCommand(util::draw::Rect({x_base, y_pos_text_box}, {x_size, util::draw::CalcTextSize(ImGui::GetFont(), font_size, description).y}, primary_color.load()));
    draw_list_->AddCommand(DrawTextLeft(y_pos_text_box, text_color.load(), description, false));
  }

  void Manager::Draw() {
    kNOTIFICATIONS->Tick();

    if (!submenus_stack_.empty()) {
      auto cur_sub = submenus_stack_.top();

      if (input_left_->Get()) {
        cur_sub->HandleKey(components::KeyInput::kLeft);
      } else if (input_right_->Get()) {
        cur_sub->HandleKey(components::KeyInput::kRight);
      } else if (input_up_->Get()) {
        cur_sub->HandleKey(components::KeyInput::kUp);
      } else if (input_down_->Get()) {
        cur_sub->HandleKey(components::KeyInput::kDown);
      } else if (input_return_->Get()) {
        cur_sub->HandleKey(components::KeyInput::kReturn);
        cur_sub = submenus_stack_.top();
        ResetSmoothScrolling();
      } else if (input_back_->Get()) {
        if (submenus_stack_.size() > 1) {
          submenus_stack_.pop();
          cur_sub = submenus_stack_.top();
          ResetSmoothScrolling();
        }
      }

      DrawHeader();
      DrawTopBar(cur_sub->GetName(), cur_sub->GetSelectedOption(), cur_sub->GetOptionCount());
      DrawBottomBar(cur_sub->GetOptionCount());
      DrawScrollBar(cur_sub->GetOptionCount(), cur_sub->GetSelectedOption());

      std::size_t draw_options_from = 0;
      std::size_t draw_options_till = cur_sub->GetOptionCount() > max_drawn_options ? max_drawn_options : cur_sub->GetOptionCount();
      if (cur_sub->GetOptionCount() > max_drawn_options && cur_sub->GetSelectedOption() >= max_drawn_options) {
        draw_options_from = cur_sub->GetSelectedOption() - max_drawn_options + 1;
        draw_options_till = cur_sub->GetSelectedOption() + 1;
      }
      size_t option_draw_count = draw_options_till - draw_options_from;

      draw_list_->AddCommand(util::draw::Rect({x_base, y_base + y_size_top_bar}, {x_size, (option_draw_count * y_size_option)}, primary_color.load()));
      for (int i = 0; draw_options_from < draw_options_till; draw_options_from++) {
        DrawOption(cur_sub->GetOption(draw_options_from), draw_options_from == cur_sub->GetSelectedOption(), i, cur_sub->GetOptionCount(), draw_options_from);

        i += 1;
      }
    }

    draw_list_->NextTargets();
  }
}