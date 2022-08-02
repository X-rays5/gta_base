//
// Created by X-ray on 3/7/2022.
//

#include "uidraw.hpp"
#include "../../ui/manager.hpp"

namespace gta_base {
  namespace scripts {
    void UiDraw::Init() {
      initialized_ = true;
    }

    void UiDraw::RunTick() {
      ui::kMANAGER->GetDrawList()->Draw();
    }
  }
}