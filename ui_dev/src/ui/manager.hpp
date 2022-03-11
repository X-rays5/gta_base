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
  class Manager {
  public:
    Manager() {
      draw_list_ = std::make_shared<draw::DrawList>();
    }

    inline void AddSubmenu(const char* name, const char* description, components::Submenus id, components::Submenu::constructor_cb constructor_cb) {
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
    std::atomic<float> menu_width = 200;
    std::atomic<float> base_x = 20;
    std::atomic<float> top_bar_y_size = 30;
    std::atomic<float> bottom_bar_y_size = 30;
    std::atomic<float> option_y_size = 45;

    std::atomic<ImColor> primary_color = ImColor(0,0,0);
    std::atomic<ImColor> secondary_color = ImColor(255,255,255);
    std::atomic<ImColor> scroller_color = ImColor(255,255,255);
    std::atomic<ImColor> text_color = ImColor(255,255,255);
  private:
    float base_y = 80;
    int max_drawn_options = 14;

    using submenu_ptr_t = std::shared_ptr<components::Submenu>;

    std::unordered_map<components::Submenus, submenu_ptr_t> submenus_;
    std::stack<submenu_ptr_t> submenus_stack_;
    std::shared_ptr<draw::DrawList> draw_list_;

  private:
    inline void DrawHeader();
    inline void DrawTopBar(const char* title, size_t option_current, size_t option_count);
    inline void DrawBottomBar(size_t option_count);
    inline void DrawOption(std::shared_ptr<components::option::BaseOption> option, bool selected = true);

  };
  inline std::unique_ptr<Manager> kMANAGER;
}
#endif //GTA_BASE_MANAGER_HPP
