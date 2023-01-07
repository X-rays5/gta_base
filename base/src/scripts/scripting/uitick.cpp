//
// Created by X-ray on 08/02/22.
//

#include "uitick.hpp"
#include "../../ui/manager.hpp"
#include "../../ui/tabs/main_tab.hpp"

namespace gta_base::scripts {
void UiTick::Init() {
  if (ui::kMANAGER) {
    initialized_ = true;

    ui::tabs::MainTab();

    const std::string build_time = std::string("Compilation time: ") + globals::compile_date + " " + globals::compile_time;
    ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "UI Loaded", build_time);
  }
}

void UiTick::RunTick() {
  ui::kMANAGER->Draw();
  ui::kMANAGER->GetDrawList()->Draw();
  ui::kMANAGER->should_tick = true;
}
}