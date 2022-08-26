//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_MANAGER_HPP
#define GTA_BASE_MANAGER_HPP
#include <string>
#include <memory>
#include <stack>
#include <robin_hood.h>
#include "imgui.h"
#include "components/submenu.hpp"
#include "notification/notification.hpp"
#include "../misc/timedinput.hpp"
#include "../d3d/draw.hpp"

namespace gta_base {
  namespace ui {
    class Manager {
    public:
      Manager();
      ~Manager();

      inline void AddSubmenu(const std::string& name, components::Submenus id, components::Submenu::constructor_cb constructor_cb) {
        auto submenu_ptr = std::make_shared<components::Submenu>(name, id, constructor_cb);
        submenus_[id] = submenu_ptr;
        if (submenus_stack_.empty()) {
          submenus_stack_.push(submenu_ptr);
        }
      }

      inline void SetActiveSubmenu(components::Submenus id) {
        auto entry = submenus_.find(id);

        if (entry != submenus_.end()) {
          submenus_stack_.push(entry->second);
        }
      }

      inline void PopSubmenu() {
        if (submenus_stack_.size() > 1) {
          submenus_stack_.pop();
        }
      }

      inline std::shared_ptr<d3d::draw::DrawList> GetDrawList() const {
        return draw_list_;
      }

      void Draw();
    public:
      std::atomic<bool> should_tick = true;
      std::atomic<bool> show_ui = false;

      std::atomic<float> x_size = 0.175f;
      std::atomic<float> x_base = 0.025f;
      std::atomic<float> x_size_scrollbar = 0.003f;
      std::atomic<float> y_size_top_bar = 0.025f;
      std::atomic<float> y_size_bottom_bar = y_size_top_bar.load();
      std::atomic<float> y_size_option = y_size_top_bar.load();
      std::atomic<float> y_offset_description = 0.005f;

      std::atomic<ImColor> primary_color = ImColor(0,0,0);
      std::atomic<ImColor> secondary_color = ImColor(255,255,255);
      std::atomic<ImColor> scroller_color = ImColor(255,255,255);
      std::atomic<ImColor> text_color = ImColor(255,255,255);
      std::atomic<ImColor> selected_text_color = ImColor(0,0,0);

      std::atomic<float> font_size = 0.018f;
    private:
      float y_base = 0.185f;
      int max_drawn_options = 14;
      float scrollbar_offset = 0.002f;

      using submenu_ptr_t = std::shared_ptr<components::Submenu>;

      robin_hood::unordered_map<components::Submenus, submenu_ptr_t> submenus_{};
      std::stack<submenu_ptr_t> submenus_stack_{};
      std::shared_ptr<d3d::draw::DrawList> draw_list_;

      std::unique_ptr<util::TimedInput> input_open_;
      std::unique_ptr<util::TimedInput> input_left_;
      std::unique_ptr<util::TimedInput> input_right_;
      std::unique_ptr<util::TimedInput> input_up_;
      std::unique_ptr<util::TimedInput> input_down_;
      std::unique_ptr<util::TimedInput> input_return_;
      std::unique_ptr<util::TimedInput> input_back_;

      int previous_selected_option_ = -1;

      bool scroller_reset_ = false;
      float scroller_speed_ = 2.5f;
      float scroller_current_pos_ = -1.f;
      bool scrollbar_reset_ = false;
      float scrollbar_speed_ = scroller_speed_;
      float scrollbar_current_pos_ = scroller_current_pos_;

      std::unique_ptr<::gta_base::ui::Notification> notification_inst_;

      float toggle_button_size_ = 0.01f;
    private:
      inline d3d::draw::Text DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center = true) const;
      inline d3d::draw::Text DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center = true) const;
      inline d3d::draw::Text DrawTextCenter(float y_pos, ImColor color, const std::string& text) const;
      inline void DrawHeader();
      inline void DrawTopBar(const std::string& title, size_t option_current, size_t option_count);
      inline void DrawBottomBar(size_t option_count);
      inline void DrawScrollBar(size_t option_count, int current_option);
      inline void DrawScrollBarScroller(float target_pos, float scroller_y_size);
      inline void DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count, size_t option_idx);
      inline bool DrawScroller(float target_pos);
      inline void DrawDescriptionText(const std::string& description, size_t option_count) const;
      inline void HandleKeyInput(std::shared_ptr<components::Submenu>& cur_sub);

      inline void ResetSmoothScrolling() {
        scroller_reset_ = true;
        scrollbar_reset_ = true;
      }
    };
    inline Manager* kMANAGER;
  }
}
#endif //GTA_BASE_MANAGER_HPP