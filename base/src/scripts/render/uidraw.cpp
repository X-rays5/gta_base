//
// Created by X-ray on 3/7/2022.
//

#include "uidraw.hpp"
#include "../../ui/manager.hpp"

namespace gta_base::scripts {
  void UiDraw::Init() {
    initialized_ = true;

    last_idx_ = ui::kMANAGER->GetDrawList()->GetRenderTarget();
  }

  void UiDraw::RunTick() {
    if (!ui::kMANAGER || !ui::kMANAGER->GetDrawList()) {
      return;
    }

    auto draw_list = ui::kMANAGER->GetDrawList();
    if (last_idx_ != draw_list->GetRenderTarget()) {
      last_idx_ = draw_list->GetRenderTarget();
      draw_list->Draw();
      ui::kMANAGER->should_tick = true;
    }
  }
}