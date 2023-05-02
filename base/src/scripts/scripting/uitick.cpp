//
// Created by X-ray on 08/02/22.
//

#include "uitick.hpp"
#include "../../ui/manager.hpp"
#include "../../ui/tabs/main_tab.hpp"

namespace gta_base::scripts {
  void UiTick::Init() {
    if (!d3d::kRENDERER) // avoid crashes since this can run too early
      return;

    if (ui::kUI_MANAGER) {
      initialized_ = true;

      //ui::tabs::MainTab();

      const std::string build_time = std::string("Compilation time: ") + globals::compile_date + " " + globals::compile_time;
      ui::kNOTIFICATIONS->Create(ui::NotificationType::kInfo, "UI Loaded", build_time);
    }
  }

  void UiTick::RunTick() {
    ui::kUI_MANAGER->Tick();
  }
}