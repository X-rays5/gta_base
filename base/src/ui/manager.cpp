//
// Created by X-ray on 02/09/23.
//

#include "manager.hpp"

namespace gta_base::ui {

  Manager::Manager() {
    ui_draw_list_ = std::make_unique<d3d::draw::DrawList>(2);
    notification_mgr_inst_ = std::make_unique<NotificationManager>(ui_draw_list_.get());
    ui_inst_ = std::make_unique<draw::UI>(ui_draw_list_.get());

    kUI_MANAGER = this;
  }

  Manager::~Manager() {
    kUI_MANAGER = nullptr;

    ui_inst_.reset();
    notification_mgr_inst_.reset();
    ui_draw_list_.reset();
  }
}