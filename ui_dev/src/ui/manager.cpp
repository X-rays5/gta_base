//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "manager.hpp"

namespace ui {
  inline draw::Text Manager::DrawTextLeft(float y_pos, ImColor color, const std::string& text) const {
    return {ImVec2(base_x + (menu_width / 38), y_pos), color, text, false, 10};
  }

  inline draw::Text Manager::DrawTextRight(float y_pos, ImColor color, const std::string& text) const {
    return {ImVec2(base_x + (menu_width - 2), y_pos), color, text, true, 10};
  }

  inline void Manager::DrawHeader() {

  }

  inline void Manager::DrawTopBar(const std::string& title, size_t option_current, size_t option_count) {
    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, base_y - top_bar_y_size), ImVec2(menu_width, top_bar_y_size), primary_color.load()));

    draw_list_->AddCommand(DrawTextLeft(base_y - top_bar_y_size, text_color.load(), title));

    std::stringstream ss;
    ss  << option_current << "/" << option_count;
    draw_list_->AddCommand(DrawTextRight(base_y - top_bar_y_size, text_color.load(), ss.str()));
  }

  inline void Manager::DrawBottomBar(size_t option_count) {
    size_t options = option_count > max_drawn_options ? max_drawn_options : option_count;

    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, base_y + (option_y_size * (float)options)), ImVec2(menu_width, bottom_bar_y_size), primary_color.load()));

    // TODO: draw some arrows pointing up and down
  }

  inline void Manager::DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos) {
    // TODO: separate the scroller from this and make a scrolling animation

    if (selected) {
      draw_list_->AddCommand(draw::Rect(ImVec2(base_x, base_y + (option_y_size * (float)option_pos)), ImVec2(menu_width, option_y_size), scroller_color.load()));
      draw_list_->AddCommand(DrawTextLeft(base_y + (option_y_size * (float)option_pos), selected_text_color.load(), option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(base_y + (option_y_size * (float)option_pos), selected_text_color.load(), option->GetRightText()));
    } else {
      draw_list_->AddCommand(draw::Rect(ImVec2(base_x, base_y + (option_y_size * (float)option_pos)), ImVec2(menu_width, option_y_size), primary_color.load()));
      draw_list_->AddCommand(DrawTextLeft(base_y + (option_y_size * (float)option_pos), text_color.load(), option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(base_y + (option_y_size * (float)option_pos), text_color.load(), option->GetRightText()));
    }

    if (selected && !option->GetDescription().empty()) {
      // TODO: draw the description of the option
    }
  }

  void Manager::Draw() {
    if (submenus_stack_.empty())
      return;

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
    } else if (input_back_->Get()) {
      if (submenus_stack_.size() > 1) {
        submenus_stack_.pop();
      }
    }

    DrawHeader();
    DrawTopBar(cur_sub->GetName(), cur_sub->GetSelectedOption() + 1, cur_sub->GetOptionCount());
    DrawBottomBar(cur_sub->GetOptionCount());

    // TODO: maybe add a scroll bar sometime

    std::size_t draw_options_from = 0;
    std::size_t draw_options_till = cur_sub->GetOptionCount() > max_drawn_options ? max_drawn_options : cur_sub->GetOptionCount();
    if (cur_sub->GetOptionCount() > max_drawn_options && cur_sub->GetSelectedOption() >= max_drawn_options) {
      draw_options_from = cur_sub->GetSelectedOption() - max_drawn_options + 1;
      draw_options_till = cur_sub->GetSelectedOption() + 1;
    }

    for (int i = 0; draw_options_from < draw_options_till; draw_options_from++) {
      DrawOption(cur_sub->GetOption(draw_options_from), draw_options_from == cur_sub->GetSelectedOption(), i);

      i += 1;
    }
  }
}