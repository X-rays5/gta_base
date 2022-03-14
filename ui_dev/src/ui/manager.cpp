//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "manager.hpp"

namespace ui {
  inline draw::Text Manager::DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center) const {
    return {ImVec2(base_x + 4, y_pos), color, text, false, center, font_size};
  }

  inline draw::Text Manager::DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center) const {
    return {ImVec2(base_x + (menu_width - 4), y_pos), color, text, true, center, font_size};
  }

  inline std::string Trim(std::string str, char trim_c) {
    while(str.front() == trim_c) {
      str.erase(0, 1);
    }
    return str;
  }

  inline std::string TrimR(std::string str, char trim_c) {
    while (str.back() == trim_c) {
      str.pop_back();
    }
    return str;
  }
  #define TRIM_STR(str, c) TrimR(Trim(str, c), c)

  __forceinline int WordWrap(std::string& wrap, float max_width, float font_size, int max_lines = 3) {
    wrap = TRIM_STR(wrap, ' ');
    float character_x_size = draw::CalcTextSize(ImGui::GetFont(), font_size, wrap).x / (float)wrap.size();
    int max_characters_per_line = std::floor(max_width / character_x_size);
    if (wrap.size() <= max_characters_per_line) {
      return 1;
    } else {
      int i = 0;
      do {
        if (wrap.size() <= (max_characters_per_line * (i + 1)))
          return i + 1;

        size_t last_space = wrap.rfind(' ', (max_characters_per_line - 1) * (i + 1));
        if (last_space == std::string::npos) {
          last_space = max_characters_per_line;
        }
        std::string front = TRIM_STR(wrap.substr(0, last_space), ' ');
        std::string back = TRIM_STR(wrap.substr(last_space), ' ');

        front.append("\n");
        front.append(back);
        wrap = front;

        i += 1;
      } while(i < (max_lines - 1));

      if (wrap.size() <= (max_characters_per_line * (i + 1)))
        return i + 1;

      // doesn't fit into max_lines lines
      int last_line_pos = wrap.rfind('\n');
      auto last_line = TRIM_STR(wrap.substr(last_line_pos), ' ');
      last_line.erase(max_characters_per_line);
      last_line.replace(last_line.size() - 3, 3, "...");
      wrap.replace(last_line_pos, wrap.size(), last_line);

      return max_lines;
    }
  }

  inline void Manager::DrawDescriptionText(std::string description, size_t option_count) const {
    float draw_zone_left = base_x + 4;
    float draw_zone_right = base_x + (menu_width - 4);

    std::cout << '\n';
    int lines = WordWrap(description, draw_zone_right - draw_zone_left, font_size);

    constexpr float separator_y_size = 5;
    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, (base_y + (option_y_size * (float)option_count)) + (bottom_bar_y_size + description_offset_y)), ImVec2(menu_width, separator_y_size), secondary_color.load()));

    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, (base_y + (option_y_size * (float)option_count)) + (bottom_bar_y_size + description_offset_y) + separator_y_size), ImVec2(menu_width, ((font_size + draw::Scale(6)) * lines)), primary_color.load()));
    draw_list_->AddCommand(DrawTextLeft(base_y + (((option_y_size * (float)option_count) + bottom_bar_y_size) + (description_offset_y + separator_y_size) + 6.f), text_color.load(), description, false));
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
    draw_list_->AddCommand(DrawTextRight(base_y + (option_y_size * (float)options), text_color.load(), "v1.0.0"));

    // TODO: draw some arrows pointing up and down
  }

  // scale number based of fps
  inline float ScaleFps(float number) {
    return number * (144.f / ImGui::GetIO().Framerate);
  }

  // FIXME: with the current implementation this works better the higher the fps
  // should correct this to something better some time
  inline void Manager::DrawScroller(float target_pos) {
    if (current_pos_ == -1.f)
      current_pos_ = target_pos;

    if (current_pos_ < target_pos) {
      current_pos_ += ScaleFps(scroller_speed_);
      if (current_pos_ > target_pos)
        current_pos_ = target_pos;
    } else if (current_pos_ > target_pos) {
      current_pos_ -= ScaleFps(scroller_speed_);
      if (current_pos_ < target_pos)
        current_pos_ = target_pos;
    }

    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, current_pos_), ImVec2(menu_width, option_y_size), scroller_color.load()));
  }

  inline void Manager::DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count) {
    draw_list_->AddCommand(draw::Rect(ImVec2(base_x, base_y + (option_y_size * (float)option_pos)), ImVec2(menu_width, option_y_size), primary_color.load()));
    if (selected) {
      DrawScroller(base_y + (option_y_size * (float)option_pos));

      draw_list_->AddCommand(DrawTextLeft(base_y + (option_y_size * (float)option_pos), selected_text_color.load(), option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(base_y + (option_y_size * (float)option_pos), selected_text_color.load(), option->GetRightText()));
    } else {
      draw_list_->AddCommand(DrawTextLeft(base_y + (option_y_size * (float)option_pos), text_color.load(), option->GetName()));

      // TODO: check if icon or text should be drawn on the right
      draw_list_->AddCommand(DrawTextRight(base_y + (option_y_size * (float)option_pos), text_color.load(), option->GetRightText()));
    }

    if (selected && !option->GetDescription().empty()) {
      DrawDescriptionText(option->GetDescription(), sub_option_count > max_drawn_options ? max_drawn_options : sub_option_count);
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
      DrawOption(cur_sub->GetOption(draw_options_from), draw_options_from == cur_sub->GetSelectedOption(), i, cur_sub->GetOptionCount());

      i += 1;
    }
  }
}