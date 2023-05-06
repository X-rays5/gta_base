//
// Created by X-ray on 3/7/2022.
//

#include "uidraw.hpp"
#include "../../ui/manager.hpp"
#include "../../ui/console/on_screen_console.hpp"

namespace gta_base::scripts {
  void UiDraw::Init() {
    initialized_ = true;
  }

  void UiDraw::RunTick() {
    return;

    auto draw_list = ui::kUI_MANAGER->GetDrawList();
    if (!ui::kUI_MANAGER || !draw_list) {
      LOG_WARN("kUI_MANAGER or draw_list is not initialized");
      return;
    }

    if (ui::kON_SCREEN_CONSOLE)
      ui::kON_SCREEN_CONSOLE->Tick(draw_list);

    if (last_idx_ != draw_list->GetRenderTarget()) {
      last_idx_ = draw_list->GetRenderTarget();
      draw_list->Draw();
    }
  }
}