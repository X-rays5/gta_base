//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
#define GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
#include "../../d3d/draw.hpp"
#include "theme.hpp"

namespace gta_base::ui::draw {
  class UI {
  public:
    explicit UI(d3d::draw::DrawList* draw_list);
    ~UI();
    
    inline Theme* GetTheme() {
      return &theme_;
    }

  private:
    d3d::draw::DrawList* draw_list_;
    Theme theme_;

  private:

  };

  inline UI* kUI{};
}
#endif //GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
