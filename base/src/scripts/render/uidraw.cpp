//
// Created by X-ray on 3/7/2022.
//

#include "uidraw.hpp"
#include "../../ui/manager.hpp"

namespace gta_base::scripts {
  void UiDraw::Init() {
    initialized_ = true;

    last_idx_ = ui::kUI_MANAGER->GetDrawList()->GetRenderTarget();
  }

  void UiDraw::RunTick() {
    if (!ui::kUI_MANAGER || !ui::kUI_MANAGER->GetDrawList()) {
      return;
    }

    auto draw_list = ui::kUI_MANAGER->GetDrawList();
    if (last_idx_ != draw_list->GetRenderTarget()) {
      last_idx_ = draw_list->GetRenderTarget();
      draw_list->Draw();
    }
  }
}