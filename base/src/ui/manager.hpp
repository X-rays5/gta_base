//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_MANAGER_EB1672026794451993E87E1E5C5CA182_HPP
#define GTA_BASE_MANAGER_EB1672026794451993E87E1E5C5CA182_HPP
#include <memory>
#include "console/on_screen_console.hpp"
#include "notification/notification.hpp"
#include "draw/ui.hpp"
#include "../d3d/draw.hpp"

namespace gta_base::ui {
  class Manager {
  public:
    Manager();
    ~Manager();

    FORCE_INLINE d3d::draw::DrawList* GetDrawList() {
      return ui_draw_list_.get();
    }

    FORCE_INLINE draw::UI* GetUI() {
      return ui_inst_.get();
    }

    FORCE_INLINE void Tick() {
      ui_inst_->Tick();
      notification_mgr_inst_->Tick();
    }

  private:
    std::unique_ptr<d3d::draw::DrawList> ui_draw_list_;
    std::unique_ptr<NotificationManager> notification_mgr_inst_;
    std::unique_ptr<draw::UI> ui_inst_;

  private:

  };

  inline Manager* kUI_MANAGER{};
}
#endif //GTA_BASE_MANAGER_EB1672026794451993E87E1E5C5CA182_HPP
