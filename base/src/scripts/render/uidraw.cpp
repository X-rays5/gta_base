//
// Created by X-ray on 3/7/2022.
//

#include "uidraw.hpp"
#include "../../ui/manager.hpp"
#include "../../ui/console/on_screen_console.hpp"

namespace gta_base::scripts {
  void UiDraw::Init() {
    initialized_ = true;

    last_idx_ = ui::kUI_MANAGER->GetDrawList()->GetRenderTarget();
  }

  void UiDraw::RunTick() {
    auto draw_list = ui::kUI_MANAGER->GetDrawList();
    if (!ui::kUI_MANAGER || !draw_list) {
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