//
// Created by X-ray on 3/9/2022.
//

#include "manager.hpp"

namespace ui {
  inline void Manager::DrawHeader() {

  }

  inline void Manager::DrawTopBar(const char* title, size_t option_current, size_t option_count) {
    draw_list_->AddCommand<draw::Rect>(ImVec2(base_x, base_y - top_bar_y_size), ImVec2(menu_width, top_bar_y_size), primary_color.load());

    draw_list_->AddCommand<draw::Text>(ImVec2(base_x + (menu_width / 10), base_y - (top_bar_y_size / 2)), text_color.load(), title, false, 10);

    std::stringstream ss;
    ss  << option_current << "/" << option_count;
    draw_list_->AddCommand<draw::Text>(ImVec2(base_x + (menu_width - 10), base_y - (top_bar_y_size / 2)), text_color.load(), ss.str().c_str(), true, 10);
  }

  inline void Manager::DrawBottomBar(size_t option_count) {
    size_t options = option_count > max_drawn_options ? max_drawn_options : option_count;

    draw_list_->AddCommand<draw::Rect>(ImVec2(base_x, base_y + (option_y_size * (float)options)), ImVec2(menu_width, top_bar_y_size), primary_color.load());

    // TODO: draw some arrows pointing up and down
  }

  inline void Manager::DrawOption(std::shared_ptr<components::option::BaseOption> option, bool selected) {

  }

  void Manager::Draw() {
    if (submenus_stack_.empty())
      return;

    auto cur_sub = submenus_stack_.top();

    DrawHeader();
    DrawTopBar(cur_sub->GetName(), cur_sub->GetSelectedOption(), cur_sub->GetOptionCount());
    DrawBottomBar(cur_sub->GetOptionCount());

    // TODO: maybe add a scroll bar sometime


  }
}