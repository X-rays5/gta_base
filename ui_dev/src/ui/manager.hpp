//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_MANAGER_HPP
#define GTA_BASE_MANAGER_HPP
#include <memory>
#include <unordered_map>
#include <stack>
#include "components/option/baseoption.hpp"
#include "components/submenu.hpp"
#include "components/enums.hpp"
#include "draw.hpp"

namespace ui {
  class TimedInput {
  public:
    TimedInput(int input, const std::chrono::milliseconds &timeout) : input_(input), timeout_(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(timeout)) {}

    bool Get() {
      if ((std::chrono::high_resolution_clock::now().time_since_epoch() - last_input_.time_since_epoch()).count() > timeout_.count()) {
        if (GetAsyncKeyState(input_)){
          last_input_ = std::chrono::high_resolution_clock::now();
          return true;
        }
      }
      return false;
    }

    int input_;
    std::chrono::high_resolution_clock::time_point last_input_;
    std::chrono::high_resolution_clock::duration timeout_;
  };

  class Manager {
  public:
    Manager() {
      draw_list_ = std::make_shared<draw::DrawList>();
      input_left_ = std::make_shared<TimedInput>(VK_LEFT, std::chrono::milliseconds(140));
      input_right_ = std::make_shared<TimedInput>(VK_RIGHT, std::chrono::milliseconds(140));
      input_up_ = std::make_shared<TimedInput>(VK_UP, std::chrono::milliseconds(120));
      input_down_ = std::make_shared<TimedInput>(VK_DOWN, std::chrono::milliseconds(120));
      input_return_ = std::make_shared<TimedInput>(VK_RETURN, std::chrono::milliseconds(300));
      input_back_ = std::make_shared<TimedInput>(VK_BACK, std::chrono::milliseconds(300));
    }

    inline void AddSubmenu(const std::string& name, const std::string& description, components::Submenus id, components::Submenu::constructor_cb constructor_cb) {
      auto submenu_ptr = std::make_shared<components::Submenu>(name, description, id, constructor_cb);
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

    inline std::shared_ptr<draw::DrawList> GetDrawList() const {
      return draw_list_;
    }

    void Draw();
  public:
    std::atomic<float> menu_width = 170;
    std::atomic<float> base_x = 20;
    std::atomic<float> top_bar_y_size = 30;
    std::atomic<float> bottom_bar_y_size = 30;
    std::atomic<float> option_y_size = 30;

    std::atomic<ImColor> primary_color = ImColor(0,0,0);
    std::atomic<ImColor> secondary_color = ImColor(255,255,255);
    std::atomic<ImColor> scroller_color = ImColor(255,255,255);
    std::atomic<ImColor> text_color = ImColor(255,255,255);
    std::atomic<ImColor> selected_text_color = ImColor(0,0,0);
  private:
    float base_y = 200;
    int max_drawn_options = 14;

    using submenu_ptr_t = std::shared_ptr<components::Submenu>;

    std::unordered_map<components::Submenus, submenu_ptr_t> submenus_;
    std::stack<submenu_ptr_t> submenus_stack_;
    std::shared_ptr<draw::DrawList> draw_list_;

    std::shared_ptr<TimedInput> input_left_;
    std::shared_ptr<TimedInput> input_right_;
    std::shared_ptr<TimedInput> input_up_;
    std::shared_ptr<TimedInput> input_down_;
    std::shared_ptr<TimedInput> input_return_;
    std::shared_ptr<TimedInput> input_back_;

  private:
    inline draw::Text DrawTextLeft(float y_pos, ImColor color, const std::string& text) const;
    inline draw::Text DrawTextRight(float y_pos, ImColor color, const std::string& text) const;
    inline void DrawHeader();
    inline void DrawTopBar(const std::string& title, size_t option_current, size_t option_count);
    inline void DrawBottomBar(size_t option_count);
    inline void DrawOption(const std::shared_ptr<components::option::BaseOption>& option, bool selected, size_t option_pos);

  };
  inline std::unique_ptr<Manager> kMANAGER;
}
#endif //GTA_BASE_MANAGER_HPP
