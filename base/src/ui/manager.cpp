//
// Created by X-ray on 3/9/2022.
//

#include "manager.hpp"
#include "../d3d/texturemanager.hpp"
#include "../logger/logger.hpp"

namespace gta_base {
  namespace ui {
    inline float ScaleFps(float number) {
      return number * (144.f / (ImGui::GetIO().Framerate / 2)); // framerate is 2x for some reason
    }

    Manager::Manager() {
      draw_list_ = std::make_shared<d3d::draw::DrawList>(3);
      input_open_ = std::make_unique<util::TimedInput>(VK_F4, 200);
      input_left_ = std::make_unique<util::TimedInput>(VK_LEFT, 140);
      input_right_ = std::make_unique<util::TimedInput>(VK_RIGHT, 140);
      input_up_ = std::make_unique<util::TimedInput>(VK_UP, 100);
      input_down_ = std::make_unique<util::TimedInput>(VK_DOWN, 100);
      input_return_ = std::make_unique<util::TimedInput>(VK_RETURN, 300);
      input_back_ = std::make_unique<util::TimedInput>(VK_BACK, 300);

      notification_inst_ = std::make_unique<Notification>();
      kMANAGER = this;
    }

    Manager::~Manager() {
      notification_inst_.reset();
      kMANAGER = nullptr;
    }

    inline d3d::draw::Text Manager::DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center) const {
      return {{x_base + 0.002f, y_pos}, color, text, false, center, font_size};
    }

    inline d3d::draw::Text Manager::DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center) const {
      return {{x_base + x_size - 0.002f, y_pos}, color, text, true, center, font_size};
    }

    inline d3d::draw::Text Manager::DrawTextCenter(float y_pos, ImColor color, const std::string& text) const {
      auto text_size = d3d::draw::CalcTextSize(ImGui::GetFont(), font_size, text);
      return {{x_base + (x_size - text_size.x) / 2, y_pos}, color, text, false, true, font_size};
    }

    void Manager::DrawHeader() {

    }

    void Manager::DrawTopBar(const std::string& title, size_t option_current, size_t option_count) {
      draw_list_->AddCommand(d3d::draw::Rect({x_base, y_base}, {x_size, y_size_top_bar}, primary_color.load()));
      draw_list_->AddCommand(d3d::draw::Rect({x_base, (y_base + y_size_top_bar) - (y_size_top_bar / 10)}, {x_size, y_size_top_bar / 10}, secondary_color.load()));
      draw_list_->AddCommand(DrawTextLeft(y_base - (y_size_top_bar / 4), text_color.load(), title));

      std::stringstream option_count_str;
      option_count_str << option_current + 1 << '/' << option_count;
      draw_list_->AddCommand(DrawTextRight(y_base - (y_size_top_bar / 4), text_color.load(), option_count_str.str()));
    }

    void Manager::DrawBottomBar(size_t option_count) {
      std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;

      float y_pos = (y_base + y_size_top_bar) + (y_size_option * visible_options);

      draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_bottom_bar}, primary_color.load()));
      draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_bottom_bar / 10}, secondary_color.load()));
      draw_list_->AddCommand(DrawTextRight(y_pos - (y_size_bottom_bar / 4), text_color.load(), "v1.0.0")); // TODO: replace this with an a real version string or something
      draw_list_->AddCommand(DrawTextLeft(y_pos - (y_size_bottom_bar / 4), text_color.load(), fmt::format("FPS: {}", (int)(ImGui::GetIO().Framerate / 2))));
    }

    void Manager::DrawScrollBar(size_t option_count, int current_option) {
      std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;
      float scrollbar_y_area = (y_size_option * visible_options);

      draw_list_->AddCommand(d3d::draw::Rect({x_base + (x_size + scrollbar_offset), y_base + y_size_top_bar}, {x_size_scrollbar, scrollbar_y_area}, primary_color.load()));

      float scroller_y_size = scrollbar_y_area / option_count;
      DrawScrollBarScroller(y_base + (current_option * scroller_y_size) + y_size_top_bar, scroller_y_size);
    }

    void Manager::DrawScrollBarScroller(float target_pos, float scroller_y_size) {
      if (scroller_current_pos_ == -1.f || scrollbar_reset_) {
        scrollbar_reset_ = false;
        scrollbar_current_pos_ = target_pos;
      }

      if (scrollbar_current_pos_ < target_pos) {
        scrollbar_current_pos_ += d3d::draw::ScaleYFromScreen(ScaleFps(scrollbar_speed_));
        if (scrollbar_current_pos_ > target_pos)
          scrollbar_current_pos_ = target_pos;
      } else if (scrollbar_current_pos_ > target_pos) {
        scrollbar_current_pos_ -= d3d::draw::ScaleYFromScreen(ScaleFps(scrollbar_speed_));
        if (scrollbar_current_pos_ < target_pos)
          scrollbar_current_pos_ = target_pos;
      }

      draw_list_->AddCommand(d3d::draw::Rect({x_base + (x_size + scrollbar_offset), scrollbar_current_pos_}, {x_size_scrollbar, scroller_y_size}, secondary_color.load()));
    }

    void Manager::DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count, size_t option_idx) {
      float pos = y_base + (y_size_option * option_pos) + y_size_top_bar;
      float text_pos = pos - (y_size_option / 4);

      ImColor text_color_tmp = text_color;
      if (selected) {
        if (previous_selected_option_ == 0 && option_idx == (sub_option_count - 1)) {
          ResetSmoothScrolling();
        } else if (previous_selected_option_ == (sub_option_count - 1) && option_idx == 0) {
          ResetSmoothScrolling();
        }
        previous_selected_option_ = (int)option_idx;

        if (DrawScroller(pos))
          text_color_tmp = selected_text_color;
      }

      auto name = option->GetName();
      auto center_text = option->GetCenterText();
      auto right_text = option->GetRightText();

      if (!center_text.empty()) {
        draw_list_->AddCommand(DrawTextCenter(text_pos, text_color_tmp, center_text));
      } else {
        if (!name.empty())
          draw_list_->AddCommand(DrawTextLeft(text_pos, text_color_tmp, name));

        if (option->HasFlag(components::OptionFlag::kToggle)) {
          std::string filepath;
          if (option->HasFlag(components::OptionFlag::kToggled)) {
            filepath = selected ? "shop_box_tickb.png" : "shop_box_tick.png";
          } else {
            filepath = selected ? "shop_box_blankb.png" : "shop_box_blank.png";
          }

          auto t = d3d::draw::ScaleYToScreen(0.05f);
          ImVec2 checkbox_size = d3d::draw::ScaleSquare(0.05f); // TODO: draw image
          //draw_list_->AddCommand(d3d::draw::Image(d3d::kTEXTURE_MANAGER->Get(filepath).texture, {x_base + (x_size - checkbox_size.x), pos - (checkbox_size.y / 4)}, checkbox_size));
        } else if (!right_text.empty()) {
          draw_list_->AddCommand(DrawTextRight(text_pos, text_color_tmp, right_text));
        }
      }

      if (selected && !option->GetDescription().empty()) {
        DrawDescriptionText(option->GetDescription(), sub_option_count > max_drawn_options ? max_drawn_options : sub_option_count);
      }
    }

    bool Manager::DrawScroller(float target_pos) {
      if (scroller_current_pos_ == -1.f || scroller_reset_){
        scroller_reset_ = false;
        scroller_current_pos_ = target_pos;
      }
      LOG_DEBUG << scroller_current_pos_ << " " << target_pos << " " << d3d::draw::ScaleYFromScreen(ScaleFps(scroller_speed_));

      if (scroller_current_pos_ < target_pos) {
        scroller_current_pos_ += d3d::draw::ScaleYFromScreen(ScaleFps(scroller_speed_));
        if (scroller_current_pos_ > target_pos)
          scroller_current_pos_ = target_pos;
      } else if (scroller_current_pos_ > target_pos) {
        scroller_current_pos_ -= d3d::draw::ScaleYFromScreen(ScaleFps(scroller_speed_));
        if (scroller_current_pos_ < target_pos)
          scroller_current_pos_ = target_pos;
      }

      draw_list_->AddCommand(d3d::draw::Rect({x_base, scroller_current_pos_}, {x_size, y_size_option}, scroller_color.load()));

      return (scroller_current_pos_ > (target_pos - (y_size_option / 2.f)) && scroller_current_pos_ < (target_pos + (y_size_option / 2.f)));
    }

    void Manager::DrawDescriptionText(const std::string& description, size_t option_count) const {
      constexpr static const float y_size_separator = 0.0046f;
      float y_pos = (y_base + (y_size_option * option_count)) + (y_size_bottom_bar + y_offset_description) + y_size_top_bar;
      float y_pos_text_box = y_pos + y_size_separator;

      std::string description_tmp = description;
      d3d::draw::WordWrap(font_size, description_tmp, x_size + 0.02f, 3); // + 0.02f is needed to avoid unused space at the end of the text box

      draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_separator}, secondary_color.load()));
      draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos_text_box}, {x_size, d3d::draw::CalcTextSize(ImGui::GetFont(), font_size, description_tmp).y}, primary_color.load()));
      draw_list_->AddCommand(DrawTextLeft(y_pos_text_box, text_color.load(), description_tmp, false));
    }

    void Manager::Draw() {
      kNOTIFICATIONS->Tick();

      if (!submenus_stack_.empty()) {
        auto cur_sub = submenus_stack_.top();
        if (input_open_->Get())
          show_ui_ = !show_ui_;

        if (show_ui_) {
          if (input_left_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kLeft);
          } else if (input_right_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kRight);
          } else if (input_up_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kUp);

            // The only way this can result in an infinite loop is if the developer is stupid.
            auto cur_opt = cur_sub->GetOption(cur_sub->GetSelectedOption());
            while(cur_opt->HasFlag(components::OptionFlag::kLabel)) {
              ResetSmoothScrolling();
              cur_sub->HandleKey(components::KeyInput::kUp);
              cur_opt = cur_sub->GetOption(cur_sub->GetSelectedOption());
            }
          } else if (input_down_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kDown);

            // The only way this can result in an infinite loop is if the developer is stupid.
            auto cur_opt = cur_sub->GetOption(cur_sub->GetSelectedOption());
            while(cur_opt->HasFlag(components::OptionFlag::kLabel)) {
              ResetSmoothScrolling();
              cur_sub->HandleKey(components::KeyInput::kDown);
              cur_opt = cur_sub->GetOption(cur_sub->GetSelectedOption());
            }
          } else if (input_return_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kReturn);
            cur_sub = submenus_stack_.top();
            ResetSmoothScrolling();
          } else if (input_back_->Get()) {
            cur_sub->HandleKey(components::KeyInput::kBackspace);
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

          draw_list_->AddCommand(d3d::draw::Rect({x_base, y_base + y_size_top_bar}, {x_size, (option_draw_count * y_size_option)}, primary_color.load()));
          for (int i = 0; draw_options_from < draw_options_till; draw_options_from++) {
            DrawOption(cur_sub->GetOption(draw_options_from), draw_options_from == cur_sub->GetSelectedOption(), i, cur_sub->GetOptionCount(), draw_options_from);

            i += 1;
          }
        }
      }

      draw_list_->NextTargets();
    }
  }
}