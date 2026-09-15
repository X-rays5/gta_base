//
// Created by X-ray on 11/09/2026.
//

#pragma once

namespace base::menu::ui {
  class Submenu;
}

namespace base::menu::ui::layout {
  /**
   * The AngelScript side of the menu: the scripts found on disk, and a submenu for each of them
   * showing what it is and offering the load and unload actions.
   */
  void InitScriptLayout();

  /**
   * Adds one row to `home` per script submenu registered as a root, below everything the menu put
   * there itself.
   *
   * Called from the root page's own update callback rather than once at startup, because the rows are
   * rebuilt every frame: a script that registers or removes a page is then reflected without anybody
   * having to be told about it, and an unloaded script's page disappears from the list with it.
   */
  void AddScriptRootSubmenus(Submenu* home);
}
