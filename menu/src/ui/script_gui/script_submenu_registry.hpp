//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include "script_submenu.hpp"
#include "submenu_id.hpp"

#include <base-common/concurrency/spinlock.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace base::menu::as::script {
  class Script;
}

namespace base::menu::ui {
  class Submenu;
}

namespace base::menu::ui::script_gui {
  /**
   * Every page a script has made, which is the one place they are known.
   *
   * A script's page is registered with the renderer under a key of its own and forgotten again when the
   * script that made it unloads, and both of those are this class's business rather than the renderer's:
   * the renderer holds submenus by key and does not care where a key came from, so the id numbering, the
   * ownership and the cleanup all live here.
   *
   * The pages are kept in the order they were made, which is the order the root page lists them in.
   *
   * Lock order, from the top: this class's lock, then a Submenu's own spinlock, then the renderer's
   * submenus_lock_. Nothing here holds this lock across a call into the renderer, and the build path -
   * which runs under a Submenu's spinlock - never touches this class's lock at all.
   */
  class ScriptSubmenuRegistry {
  public:
    /// Installs itself as kSCRIPT_SUBMENUS, which is how a script's `gui::Submenu` finds the registry
    /// there is only ever one of.
    ScriptSubmenuRegistry();
    ~ScriptSubmenuRegistry();

    ScriptSubmenuRegistry(const ScriptSubmenuRegistry&) = delete;
    ScriptSubmenuRegistry(ScriptSubmenuRegistry&&) = delete;
    ScriptSubmenuRegistry& operator=(const ScriptSubmenuRegistry&) = delete;
    ScriptSubmenuRegistry& operator=(ScriptSubmenuRegistry&&) = delete;

    /**
     * A new page, numbered from the id allocator and registered with the renderer under its key.
     *
     * The renderer is given every page, not only the ones that go on to be roots: a link names the page
     * it walks into by key, so a page the renderer does not hold is a page nothing can navigate to.
     * Which pages are *listed* on the root page of the menu is RegisterRoot's business, and the
     * distinction between the two is why the registration happens here and the listing there.
     */
    [[nodiscard]] std::shared_ptr<ScriptSubmenu> Create(std::string name_key, std::string owner_name,
                                                        std::weak_ptr<as::script::Script> owner,
                                                        std::string ui_declaration);

    /**
     * Puts the page on the root page of the menu, below everything the menu put there itself, under the
     * name of the script that made it.
     *
     * A script has one root: the row is where a player finds everything that script offers, and its
     * other pages hang below that row rather than beside it. So this is false, and reports, for a
     * second page of the same script - which is a page to link to from the first rather than to list
     * again. Idempotent - a script that registers the same page on every load gets one entry - and
     * false for a page that has been removed, since registering it again would put a dead id in front
     * of the player.
     */
    bool RegisterRoot(const std::shared_ptr<ScriptSubmenu>& sub);

    /**
     * Takes a page away: the renderer is told to forget the key, the id is released, and the page is
     * marked gone. False when it was not registered, which makes a second Remove harmless.
     *
     * A player standing on the page is put back on the one underneath by the renderer, which is what
     * keeps a script from stranding the menu on an id nothing answers to.
     */
    bool Remove(const std::shared_ptr<ScriptSubmenu>& sub);

    /**
     * Takes away every page of the script called `script_name`. This is what a script's own destruction
     * calls, since a script that unloads is not one that tidies up after itself.
     */
    void RemoveOwnedBy(std::string_view script_name);

    /**
     * Adds one row to `home` per registered root, in the order they were made, each named after the
     * script that made it.
     *
     * Called from the root page's own update callback, so the rows are rebuilt every frame and a script
     * that registers or removes a page is reflected without anybody being told. The name is passed as
     * the text itself rather than as a translation key, since a script's name is a literal: the
     * component localizes what it is given, a miss shows as written, and the prefix a script's own keys
     * go under is not one its bare name can be.
     */
    void BuildRootLinks(Submenu* home);

    /// How many pages exist right now. Diagnostics and tests.
    [[nodiscard]] std::size_t Count() const;

    /**
     * Whether `id` is one the allocator has handed out and not taken back. Diagnostics and tests.
     *
     * This is the only way from outside to see that a page which went really released its id rather than
     * leaving the allocator believing a page that no longer exists is still in front of a player.
     */
    [[nodiscard]] bool IsOutstanding(std::uint64_t id) const {
      return ids_.IsOutstanding(id);
    }

    /**
     * Every page currently made, in the order they were made. Diagnostics and tests.
     *
     * A copy, taken under the lock, because what a caller does with the pages is not this class's
     * business and holding the lock across it would be. The renderer is what reaches a page in the game,
     * through the key it was registered under, so this exists for the case the renderer cannot serve:
     * a harness with no renderer at all, which still needs to build a page to see what it drew.
     */
    [[nodiscard]] std::vector<std::shared_ptr<ScriptSubmenu>> Pages() const;

    /// The page being built on this thread, or null - see ScriptSubmenu::CurrentBuild.
    [[nodiscard]] static ScriptSubmenu* CurrentBuild();

  private:
    struct Entry {
      std::shared_ptr<ScriptSubmenu> sub;
      /// Whether the page is listed on the root page of the menu, under its script's name - which is
      /// true of at most one page per script. A page that is not is still navigable from another page of
      /// the same script; it simply has no link of its own.
      bool root = false;
    };

    /// Erases `sub` from the registry and hands back whether it was there. The caller does the rest,
    /// which is what keeps the renderer out of this class's lock.
    bool TakeLocked(const std::shared_ptr<ScriptSubmenu>& sub);

    mutable common::concurrency::Spinlock lock_;
    ScriptSubmenuIdAllocator ids_;
    std::vector<Entry> subs_;
  };

  inline ScriptSubmenuRegistry* kSCRIPT_SUBMENUS{};
}
