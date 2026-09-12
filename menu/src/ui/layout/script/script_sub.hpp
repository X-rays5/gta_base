//
// Created by X-ray on 11/09/2026.
//

#pragma once

namespace base::menu::ui::layout {
  /**
   * The AngelScript side of the menu: the scripts found on disk, and a submenu for each of them
   * showing what it is and offering the load and unload actions - the same shape the Lua layout has.
   */
  void InitScriptLayout();
}
