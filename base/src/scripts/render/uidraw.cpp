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
    /*if (last_idx_ != ui::kMANAGER->GetDrawList()->GetRenderTarget()) {
      ui::kMANAGER->GetDrawList()->Draw();
      ui::kMANAGER->should_tick = true;

      last_idx_ = ui::kMANAGER->GetDrawList()->GetRenderTarget();
    }*/
  }
}