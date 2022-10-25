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
#include "translation/translation_manager.hpp"
#include "../misc/hotkey_manager.hpp"
#include "../misc/timedinput.hpp"
#include "../d3d/draw.hpp"

namespace gta_base::ui {
    class Manager {
    public:
      Manager();
      ~Manager();

      inline void AddSubmenu(Submenus id, const std::string& name_key, Submenu::constructor_cb cb) {
        std::unique_lock lock(sub_mtx_);
        auto submenu_ptr = std::make_shared<Submenu>(name_key, std::move(cb));
        submenus_[id] = submenu_ptr;
        if (submenus_stack_.empty()) {
          submenus_stack_.push(submenu_ptr);
        }
      }

      inline void SetActiveSubmenu(Submenus id) {
        std::unique_lock lock(sub_mtx_);
        auto entry = submenus_.find(id);

        if (entry != submenus_.end()) {
          submenus_stack_.push(entry->second);
        }
      }

      inline std::shared_ptr<Submenu> GetSubmenu(Submenus id) {
        std::unique_lock lock(sub_mtx_);
        auto entry = submenus_.find(id);

        if (entry != submenus_.end()) {
          return entry->second;
        }

        return nullptr;
      }

      inline void PopSubmenu() {
        std::unique_lock lock(sub_mtx_);
        if (submenus_stack_.size() > 1) {
          submenus_stack_.pop();
        }
      }

      inline void IterateAllOptions(std::function<void(std::shared_ptr<Submenu>, std::shared_ptr<option::BaseOption>)> cb) {
        std::unique_lock lock(sub_mtx_);
        for (auto&& sub_pair : submenus_) {
          auto sub = sub_pair.second;

          if (sub->GetOptionCount() == 0) {
            sub->CreateOptions();
            if (sub->GetOptionCount() == 0)
              continue;
          }

          for (int i = 0; i < sub->GetOptionCount(); i++) {
            std::invoke(cb, sub, sub->GetOption(i));
          }
        }
      }

      inline misc::HotkeyManager::HotkeyPressResult HotkeyPressed(const std::string& name) {
        std::unique_lock lock(sub_mtx_);
        for (auto&& sub : submenus_) {
          auto res = sub.second->HotkeyPressed(name);
          if (res.has_value())
            return res;
        }

        return {};
      }

      inline std::shared_ptr<d3d::draw::DrawList> GetDrawList() const {
        return draw_list_;
      }

      void Draw();
    public:
      std::atomic<bool> should_tick = true;
      std::atomic<bool> show_ui = false;

      float x_size = 0.175f;
      float x_base = 0.025f;
      float x_size_scrollbar = 0.003f;
      float y_size_top_bar = 0.025f;
      float y_size_bottom_bar = y_size_top_bar;
      float y_size_option = y_size_top_bar;
      float y_offset_description = 0.005f;
      float y_base = 0.185f;
      int max_drawn_options = 14;
      int time_between_scroll_ms = 100;

      ImColor primary_color = ImColor(0,0,0);
      ImColor secondary_color = ImColor(255,255,255);
      ImColor scroller_color = ImColor(255,255,255);
      ImColor text_color = ImColor(255,255,255);
      ImColor selected_text_color = ImColor(0,0,0);

      float font_size = 0.018f;
    private:
      std::mutex sub_mtx_;
      float scrollbar_offset = 0.002f;

      using submenu_ptr_t = std::shared_ptr<Submenu>;

      robin_hood::unordered_map<Submenus, submenu_ptr_t> submenus_{};
      std::stack<submenu_ptr_t> submenus_stack_{};
      std::shared_ptr<d3d::draw::DrawList> draw_list_;

      std::unique_ptr<util::TimedInput> input_open_;
      std::unique_ptr<util::TimedInput> input_left_;
      std::unique_ptr<util::TimedInput> input_right_;
      std::unique_ptr<util::TimedInput> input_up_;
      std::unique_ptr<util::TimedInput> input_down_;
      std::unique_ptr<util::TimedInput> input_return_;
      std::unique_ptr<util::TimedInput> input_back_;
      std::unique_ptr<util::TimedInput> input_create_hotkey_;

      size_t option_before_scroll_ = -1;

      d3d::draw::Animate scroller_animation{};
      bool scroller_reset = false;
      float scroller_prev_pos = -1;
      d3d::draw::Animate scrollbar_animation{};
      bool scrollbar_reset = false;
      float scrollbar_prev_pos = -1;

      std::unique_ptr<Notification> notification_inst_;
      std::unique_ptr<TranslationManager> translation_manager_inst_;
      std::unique_ptr<misc::HotkeyManager> hotkey_manager_inst_;

      float toggle_button_size_ = 0.01f;
    private:
      inline d3d::draw::Text DrawTextLeft(float y_pos, ImColor color, const std::string& text, bool center = true, ImFont* font = nullptr) const;
      inline d3d::draw::Text DrawTextRight(float y_pos, ImColor color, const std::string& text, bool center = true, ImFont* font = nullptr) const;
      inline d3d::draw::Text DrawTextCenter(float y_pos, ImColor color, const std::string& text, ImFont* font = nullptr) const;
      inline float CalcOptPos(size_t option_pos) const;
      inline float CalcScrollbarPos(size_t option_pos, float y_size) const;
      inline void DrawHeader();
      inline void DrawTopBar(const std::string& title, size_t option_current, size_t option_count);
      inline void DrawBottomBar(size_t option_count);
      inline void DrawScrollBar(size_t option_count, int current_option);
      inline void DrawScrollBarScroller(float prev_pos, float target_pos, float scroller_y_size);
      inline void DrawOption(const std::shared_ptr<option::BaseOption>& option, bool selected, size_t option_pos, size_t sub_option_count, size_t option_idx);
      inline bool DrawScroller(float prev_pos, float target_pos);
      inline void DrawDescriptionText(const std::string& description, size_t option_count) const;
      inline void HandleKeyInput(std::shared_ptr<Submenu>& cur_sub);

      inline void ResetSmoothScrolling() {
        scroller_reset = true;
        scrollbar_reset = true;
      }
    };
    inline Manager* kMANAGER;
  }
#endif //GTA_BASE_MANAGER_HPP