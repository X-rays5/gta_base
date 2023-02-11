//
// Created by X-ray on 02/09/23.
//

#include "ui.hpp"

namespace gta_base::ui::draw {
  UI::UI(d3d::draw::DrawList* draw_list) : draw_list_(draw_list) {
    kUI = this;
  }

  UI::~UI() {
    kUI = nullptr;
  }
}