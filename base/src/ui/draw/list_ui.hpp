//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_LIST_UI_7DBD7AFA72194F909E3F8A255151C44D_HPP
#define GTA_BASE_LIST_UI_7DBD7AFA72194F909E3F8A255151C44D_HPP
#include "theme.hpp"
#include "../../d3d/draw.hpp"
#include "components/submenu.hpp"

namespace gta_base::ui::draw::list_ui {
  void Draw(Theme* theme, components::Submenu* draw_data, d3d::draw::DrawList* draw_list);
}
#endif //GTA_BASE_LIST_UI_7DBD7AFA72194F909E3F8A255151C44D_HPP
