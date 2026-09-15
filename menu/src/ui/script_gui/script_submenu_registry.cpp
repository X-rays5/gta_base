//
// Created by X-ray on 12/09/2026.
//

#include "script_submenu_registry.hpp"

#include "../components/sub_link_component.hpp"
#include "../menu_renderer.hpp"

#include <algorithm>
#include <utility>

namespace base::menu::ui::script_gui {
  ScriptSubmenuRegistry::ScriptSubmenuRegistry() {
    kSCRIPT_SUBMENUS = this;
  }

  ScriptSubmenuRegistry::~ScriptSubmenuRegistry() {
    kSCRIPT_SUBMENUS = nullptr;
  }

  std::shared_ptr<ScriptSubmenu> ScriptSubmenuRegistry::Create(std::string name_key, std::string owner_name,
                                                               std::weak_ptr<as::script::Script> owner,
                                                               std::string ui_declaration) {
    auto sub = std::make_shared<ScriptSubmenu>(ids_.Acquire(), std::move(name_key), std::move(owner_name),
                                               std::move(owner), std::move(ui_declaration));

    {
      common::concurrency::ScopedSpinlock lock(lock_);
      subs_.push_back(Entry{sub, false});
    }

    // Every page is registered with the renderer, not only the ones that become roots: the renderer is
    // what navigates, and a link names the page it goes to by its key. A page the renderer has never
    // heard of is one nothing can walk into - which is what "Submenu not found" means when it is
    // pressed - so a sub-page has to be registered here even though it is not listed on the root page.
    //
    // Outside the lock: the renderer has one of its own, and the order between the two is this class's
    // first, which only holds if one is never taken while the other is held.
    if (kMENU_RENDERER) {
      kMENU_RENDERER->AddSubmenu(sub->GetKey(), Submenu(sub->GetNameKey(), [sub](Submenu* frame) {
        sub->BuildInto(frame);
      }));
    }

    return sub;
  }

  bool ScriptSubmenuRegistry::RegisterRoot(const std::shared_ptr<ScriptSubmenu>& sub) {
    if (!sub || sub->IsRemoved()) {
      return false;
    }

    {
      common::concurrency::ScopedSpinlock lock(lock_);
      const auto it = std::ranges::find_if(subs_, [&sub](const Entry& entry) { return entry.sub == sub; });
      if (it == subs_.end()) {
        // Not one of ours: it was removed between the check above and here, or it never came from
        // Create. Either way there is no page to register.
        return false;
      }

      // Idempotent, so that a script which registers on every load does not accumulate entries.
      if (it->root) {
        return true;
      }

      // One root per script. The row on the root page is the script's own - it carries the script's
      // name, and it is where a player finds everything the script offers - so a second page
      // registering itself would be a second script's worth of rows under one name. Every other page a
      // script makes hangs below that row, reached by a link from a page of the same script.
      const auto mine = std::ranges::find_if(subs_, [&sub](const Entry& entry) {
        return entry.root && entry.sub != sub && entry.sub->GetOwnerName() == sub->GetOwnerName();
      });
      if (mine != subs_.end()) {
        LOG_ERROR("[AS] '{}' was not registered: the script that made it already has '{}' on the root page, "
                  "and a script has one root of its own - link to this page from there instead",
                  sub->GetNameKey(), mine->sub->GetNameKey());
        return false;
      }

      it->root = true;
    }

    return true;
  }

  bool ScriptSubmenuRegistry::TakeLocked(const std::shared_ptr<ScriptSubmenu>& sub) {
    return std::erase_if(subs_, [&sub](const Entry& entry) { return entry.sub == sub; }) > 0;
  }

  bool ScriptSubmenuRegistry::Remove(const std::shared_ptr<ScriptSubmenu>& sub) {
    if (!sub) {
      return false;
    }

    {
      common::concurrency::ScopedSpinlock lock(lock_);
      if (!TakeLocked(sub)) {
        return false;
      }
    }

    // Outside the lock: the renderer has a lock of its own, and the order between the two is this
    // class's first - which only holds if one is never taken while the other is held.
    sub->SetRemoved();
    ids_.Release(sub->GetId());

    if (kMENU_RENDERER) {
      kMENU_RENDERER->RemoveSubmenu(sub->GetKey());
    }

    return true;
  }

  void ScriptSubmenuRegistry::RemoveOwnedBy(const std::string_view script_name) {
    std::vector<std::shared_ptr<ScriptSubmenu>> doomed;
    {
      common::concurrency::ScopedSpinlock lock(lock_);
      std::erase_if(subs_, [&](const Entry& entry) {
        if (entry.sub->GetOwnerName() != script_name) {
          return false;
        }

        doomed.push_back(entry.sub);
        return true;
      });
    }

    for (const auto& sub : doomed) {
      sub->SetRemoved();
      ids_.Release(sub->GetId());

      if (kMENU_RENDERER) {
        // Nothing of the dying script is called back by this: removal erases the page and tells the one
        // underneath that it is current, and that is the menu's own open callback rather than a
        // script's.
        kMENU_RENDERER->RemoveSubmenu(sub->GetKey());
      }
    }
  }

  void ScriptSubmenuRegistry::BuildRootLinks(Submenu* home) {
    if (!home) {
      return;
    }

    // Taken under the lock and used outside it: this runs from inside the root page's update callback,
    // which holds that submenu's spinlock, and adding a component takes it again - so holding this
    // class's lock across the adds would be this lock under a submenu's, the order this class exists
    // to avoid.
    std::vector<Entry> roots;
    {
      common::concurrency::ScopedSpinlock lock(lock_);
      for (const auto& entry : subs_) {
        if (entry.root) {
          roots.push_back(entry);
        }
      }
    }

    for (const auto& entry : roots) {
      // The renderer-free form: the row's target is a key of this registry's making rather than one of
      // the renderer's ids, so there is nothing to look up and nothing to ask the renderer about.
      //
      // The row is the script's own, and carries the script's name rather than the name the page was
      // made with: the page's name is what the page calls itself once it is open, and the row is what
      // a player finds the script by. A translation that happens to be registered under the name - as
      // a script's own prefix makes unlikely but not impossible - is what it shows instead.
      home->AddComponent(components::SubLinkComponent(entry.sub->GetKey(), entry.sub->GetOwnerName()));
    }
  }

  std::size_t ScriptSubmenuRegistry::Count() const {
    common::concurrency::ScopedSpinlock lock(lock_);
    return subs_.size();
  }

  std::vector<std::shared_ptr<ScriptSubmenu>> ScriptSubmenuRegistry::Pages() const {
    std::vector<std::shared_ptr<ScriptSubmenu>> pages;

    common::concurrency::ScopedSpinlock lock(lock_);
    pages.reserve(subs_.size());
    for (const auto& entry : subs_) {
      pages.push_back(entry.sub);
    }

    return pages;
  }

  ScriptSubmenu* ScriptSubmenuRegistry::CurrentBuild() {
    return ScriptSubmenu::CurrentBuild();
  }
}
