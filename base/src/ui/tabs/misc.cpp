//
// Created by X-ray on 09/18/22.
//

#include "misc.hpp"
#include "tab_includes.hpp"
#include "../../rage/util/session_switcher.hpp"

namespace gta_base::ui::tabs {
void MiscTab() {
  kMANAGER->AddSubmenu(Submenus::Misc, "tab/title/misc", [](Submenu *sub) {
    sub->AddOption(option::ExecuteOption("option/skip_cutscene", "", [] {
      fiber::kPOOL->AddJob([] {
        CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
      });
    }));
  });
}
}