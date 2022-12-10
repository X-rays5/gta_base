//
// Created by X-ray on 3/9/2022.
//

#include "manager.hpp"
#include "../d3d/renderer.hpp"
#include "components/keyboard.hpp"
#include "fonts/IconsFontAwesome6.h"

namespace gta_base::ui {
  inline size_t CorrectPrevOptIdx(std::int64_t prev_opt_idx, std::int64_t selected_opt_idx, std::int64_t option_count, std::int64_t max_draw_options) {
    // make sure prev_opt_idx is in range of selected_opt_idx with the range of max_draw_options
    // but there are only option_count options
    std::int64_t res{};
    if (prev_opt_idx < selected_opt_idx) {
      res = std::clamp(prev_opt_idx, selected_opt_idx - max_draw_options, selected_opt_idx);
    } else if (prev_opt_idx > selected_opt_idx) {
      res = std::clamp(prev_opt_idx, selected_opt_idx, selected_opt_idx + max_draw_options);
    } else {
      res = prev_opt_idx;
    }

    if (prev_opt_idx > option_count) {
      res = option_count - 1;
    }

    return res;
  }

  inline void SkipLabelOpt(std::shared_ptr<Submenu>& sub, KeyInput where_to_scroll) {
    // Could cause issues when there are only label options
    auto cur_opt = sub->GetOption(sub->GetSelectedOption());
    while(cur_opt->HasFlag(OptionFlag::kLabel)) {
      sub->HandleKey(where_to_scroll);
      cur_opt = sub->GetOption(sub->GetSelectedOption());
    }
  }

  Manager::Manager() {
    draw_list_ = std::make_shared<d3d::draw::DrawList>(3);
    input_open_ = std::make_unique<util::TimedInput>(VK_F4, 200);
    input_left_ = std::make_unique<util::TimedInput>(VK_LEFT, 140);
    input_right_ = std::make_unique<util::TimedInput>(VK_RIGHT, 140);
    input_up_ = std::make_unique<util::TimedInput>(VK_UP, time_between_scroll_ms);
    input_down_ = std::make_unique<util::TimedInput>(VK_DOWN, time_between_scroll_ms);
    input_return_ = std::make_unique<util::TimedInput>(VK_RETURN, 300);
    input_back_ = std::make_unique<util::TimedInput>(VK_BACK, 300);
    input_create_hotkey_ = std::make_unique<util::TimedInput>(VK_F1, 300);
    input_modify_value_ = std::make_unique<util::ModifierTimedInput>(VK_CONTROL, VK_RETURN, 200);

    notification_inst_ = std::make_unique<Notification>();
    translation_manager_inst_ = std::make_unique<TranslationManager>(); // TODO: Set to translation from current language
    hotkey_manager_inst_ = std::make_unique<misc::HotkeyManager>();

    common::LoadImage("test.png", &img_header);

    kMANAGER = this;
  }

  Manager::~Manager() {
    notification_inst_.reset();
    translation_manager_inst_.reset();
    hotkey_manager_inst_.reset();
    kMANAGER = nullptr;
  }

  inline d3d::draw::Text Manager::DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center, ImFont* font) const {
    return {{x_base + 0.002f, y_pos}, color, text, false, center, font_size, font};
  }

  inline d3d::draw::Text Manager::DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center, ImFont* font) const {
    return {{x_base + x_size - 0.002f, y_pos}, color, text, true, center, font_size, font};
  }

  inline d3d::draw::Text Manager::DrawTextCenter(float y_pos, ImColor color, const std::string& text, ImFont* font) const {
    if (!font) {
      font = ImGui::GetFont();
    }

    auto text_size = d3d::draw::CalcTextSize(font, font_size, text);
    return {{x_base + (x_size - text_size.x) / 2, y_pos}, color, text, false, true, font_size, font};
  }

  float Manager::CalcOptPos(size_t option_pos) const {
    return y_base + (y_size_option * option_pos) + y_size_top_bar;
  }

  float Manager::CalcScrollbarPos(size_t option_pos, float y_size) const {
    return y_base + (y_size * option_pos) + y_size_top_bar;
  }

  void Manager::DrawHeader() {
    if (img_header) {
      auto y_pos = y_base - y_size_header;
      if (y_pos < 0)
        y_pos = 0;
      else if (y_pos > 1)
        y_pos = 1;

     draw_list_->AddCommand(d3d::draw::Image(img_header, {x_base, y_pos}, {x_size, y_size_header}));
    }
  }

  void Manager::DrawTopBar(const std::string& title, size_t option_current, size_t option_count) {
    draw_list_->AddCommand(d3d::draw::Rect({x_base, y_base}, {x_size, y_size_top_bar}, primary_color));
    draw_list_->AddCommand(d3d::draw::Rect({x_base, (y_base + y_size_top_bar) - (y_size_top_bar / 10)}, {x_size, y_size_top_bar / 10}, secondary_color));
    draw_list_->AddCommand(DrawTextLeft(y_base - (y_size_top_bar / 4), text_color, title, true, d3d::kRENDERER->GetFontBold()));

    std::stringstream option_count_str;
    option_count_str << option_current + 1 << '/' << option_count;
    draw_list_->AddCommand(DrawTextRight(y_base - (y_size_top_bar / 4), text_color, option_count_str.str()));
  }

  void Manager::DrawBottomBar(size_t option_count) {
    std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;

    float y_pos = (y_base + y_size_top_bar) + (y_size_option * visible_options);

    draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_bottom_bar}, primary_color));
    draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_bottom_bar / 10}, secondary_color));
    draw_list_->AddCommand(DrawTextRight(y_pos - (y_size_bottom_bar / 4), text_color, globals::version));
    draw_list_->AddCommand(DrawTextLeft(y_pos - (y_size_bottom_bar / 4), text_color, fmt::format("FPS: {}", (int)(ImGui::GetIO().Framerate / 2))));
  }

  void Manager::DrawScrollBar(size_t option_count, int current_option) {
    if (option_count <= max_drawn_options) {
      return;
    }

    std::uint32_t visible_options = option_count > max_drawn_options ? max_drawn_options : option_count;
    float scrollbar_y_area = (y_size_option * visible_options);

    draw_list_->AddCommand(d3d::draw::Rect({x_base + (x_size + scrollbar_offset), y_base + y_size_top_bar}, {x_size_scrollbar, scrollbar_y_area}, primary_color));

    float scroller_y_size = scrollbar_y_area / option_count;
    DrawScrollBarScroller(CalcScrollbarPos(option_before_scroll_, scroller_y_size), CalcScrollbarPos(current_option, scroller_y_size), scroller_y_size);
  }

  void Manager::DrawScrollBarScroller(float prev_pos, float target_pos, float scroller_y_size) {
    float pos{};
    if (prev_pos == target_pos) {
      pos = target_pos;
    } else {
      if (scrollbar_prev_pos != prev_pos) {
        scrollbar_animation = d3d::draw::Animate(prev_pos, target_pos, time_between_scroll_ms);
        scrollbar_prev_pos = prev_pos;
      }

      pos = static_cast<float>(scrollbar_animation.GetNow());
    }

    draw_list_->AddCommand(d3d::draw::Rect({x_base + (x_size + scrollbar_offset), pos}, {x_size_scrollbar, scroller_y_size}, secondary_color));
  }

  void Manager::DrawOption(const std::shared_ptr<option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count) {
    float pos = CalcOptPos(option_pos);
    float text_pos = pos - (y_size_option / 4);

    ImColor text_color_tmp = text_color;
    if (selected) {
      auto prev_opt = option_before_scroll_;
      if (prev_opt >= max_drawn_options)
        prev_opt = option_pos;
      if (DrawScroller(CalcOptPos(prev_opt), pos))
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

      if (option->HasFlag(OptionFlag::kToggle)) {
        bool toggled = option->HasFlag(OptionFlag::kToggled);

        auto toggle_text = toggled ? ICON_FA_CHECK : ICON_FA_XMARK;
        draw_list_->AddCommand(DrawTextRight(text_pos, text_color_tmp, toggle_text));
        draw_list_->AddCommand(d3d::draw::Text({(x_base + x_size) - 0.013f, text_pos}, text_color_tmp, right_text, true, true, font_size));
      } else if (!right_text.empty()) {
        draw_list_->AddCommand(DrawTextRight(text_pos, text_color_tmp, right_text));
      }
    }

    if (selected && !option->GetDescription().empty()) {
      DrawDescriptionText(option->GetDescription(), sub_option_count > max_drawn_options ? max_drawn_options : sub_option_count);
    }
  }

  bool Manager::DrawScroller(float prev_pos, float target_pos) {

    float pos{};
    if (prev_pos == target_pos) {
      pos = target_pos;
    } else {
      if (scroller_prev_pos != prev_pos) {
        scroller_animation = d3d::draw::Animate(prev_pos, target_pos, time_between_scroll_ms);
        scroller_prev_pos = prev_pos;
      }

      pos = static_cast<float>(scroller_animation.GetNow());
    }

    draw_list_->AddCommand(d3d::draw::Rect({x_base, pos}, {x_size, y_size_option}, scroller_color));

    return (pos > (target_pos - (y_size_option / 2.f)) && pos < (target_pos + (y_size_option / 2.f)));
  }

  void Manager::DrawDescriptionText(const std::string& description, size_t option_count) const {
    constexpr static const float y_size_separator = 0.0046f;
    float y_pos = (y_base + (y_size_option * option_count)) + (y_size_bottom_bar + y_offset_description) + y_size_top_bar;
    float y_pos_text_box = y_pos + y_size_separator;

    std::string description_tmp = description;
    d3d::draw::WordWrap(font_size, description_tmp, x_size + 0.01f, 3); // + 0.01f is needed to avoid unused space at the end of the text box

    draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos}, {x_size, y_size_separator}, secondary_color));
    draw_list_->AddCommand(d3d::draw::Rect({x_base, y_pos_text_box}, {x_size, d3d::draw::CalcTextSize(ImGui::GetFont(), font_size, description_tmp).y}, primary_color));
    draw_list_->AddCommand(DrawTextLeft(y_pos_text_box, text_color, description_tmp, false));
  }

  void Manager::HandleKeyInput(std::shared_ptr<Submenu>& cur_sub) {
    if (input_left_->Get()) {
      cur_sub->HandleKey(KeyInput::kLeft);
    } else if (input_right_->Get()) {
      cur_sub->HandleKey(KeyInput::kRight);
    } else if (input_up_->Get()) {
      option_before_scroll_ = cur_sub->GetSelectedOption();
      cur_sub->HandleKey(KeyInput::kUp);

      SkipLabelOpt(cur_sub, KeyInput::kUp);

      option_before_scroll_ = CorrectPrevOptIdx(option_before_scroll_, cur_sub->GetSelectedOption(), cur_sub->GetOptionCount(), max_drawn_options);
    } else if (input_down_->Get()) {
      option_before_scroll_ = cur_sub->GetSelectedOption();
      cur_sub->HandleKey(KeyInput::kDown);

      SkipLabelOpt(cur_sub, KeyInput::kDown);

      option_before_scroll_ = CorrectPrevOptIdx(option_before_scroll_, cur_sub->GetSelectedOption(), cur_sub->GetOptionCount(), max_drawn_options);
    } else if (input_return_->Get()) {
      cur_sub->HandleKey(KeyInput::kReturn);
      if (cur_sub != submenus_stack_.top()) {
        option_before_scroll_ = cur_sub->GetSelectedOption();
        cur_sub->Clear();
        cur_sub = submenus_stack_.top();

        cur_sub->CreateOptions();
        SkipLabelOpt(cur_sub, KeyInput::kDown);

        option_before_scroll_ = CorrectPrevOptIdx(option_before_scroll_, cur_sub->GetSelectedOption(), cur_sub->GetOptionCount(), max_drawn_options);
      }
    } else if (input_back_->Get()) {
      cur_sub->HandleKey(KeyInput::kBackspace);
      if (submenus_stack_.size() > 1) {
        option_before_scroll_ = cur_sub->GetSelectedOption();
        submenus_stack_.pop();
        cur_sub->Clear();

        cur_sub = submenus_stack_.top();
        cur_sub->CreateOptions();
        option_before_scroll_ = CorrectPrevOptIdx(option_before_scroll_, cur_sub->GetSelectedOption(), cur_sub->GetOptionCount(), max_drawn_options);
      } else {
        show_ui = false;
      }
    } else if (input_create_hotkey_->Get()) {
      auto cur_opt = cur_sub->GetOption(cur_sub->GetSelectedOption());
      if (cur_opt->HasFlag(OptionFlag::kHotkeyable)) {
        hotkey_manager_inst_->StartHotkeyAdd(cur_opt->GetNameKey().data());
      } else {
        notification_inst_->Create(Notification::Type::kFail, "Hotkey", "Current option doesn't support adding a hotkey");
      }
    } else if (input_modify_value_->Get()) {
      cur_sub->GetOption(cur_sub->GetSelectedOption())->HandleKey(KeyInput::kChangeValue);
    }
  }

  void Manager::Draw() {
    // Don't run again when the draw calls haven't run yet
    if (!should_tick)
      return;

    kNOTIFICATIONS->Tick();
    keyboard::kMANAGER->Tick();

    if (!submenus_stack_.empty()) {
      if (input_open_->Get())
        show_ui = !show_ui;

      if (show_ui) {
        auto cur_sub = submenus_stack_.top();
        cur_sub->Clear();
        cur_sub->CreateOptions();

        if (!keyboard::kMANAGER->KeyBoardActive())
          HandleKeyInput(cur_sub);

        if (cur_sub->GetOptionCount() == 0)
          return;

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

        draw_list_->AddCommand(d3d::draw::Rect({x_base, y_base + y_size_top_bar}, {x_size, (option_draw_count * y_size_option)}, primary_color));
        for (int i = 0; draw_options_from < draw_options_till; draw_options_from++) {
          DrawOption(cur_sub->GetOption(draw_options_from), draw_options_from == cur_sub->GetSelectedOption(), i, cur_sub->GetOptionCount());

          i += 1;
        }
      }
    }

    should_tick = false;
    draw_list_->NextTargets();
  }
}