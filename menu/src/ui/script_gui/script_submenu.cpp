//
// Created by X-ray on 12/09/2026.
//

#include "script_submenu.hpp"

#include "submenu_id.hpp"

#include <utility>

namespace base::menu::ui::script_gui {
  namespace {
    /// The page being built on this thread. One thread at a time, which is what makes it a plain
    /// pointer rather than anything shared: the render thread is the only one that builds, and a
    /// callback reached from it is on it.
    thread_local ScriptSubmenu* current_build = nullptr;

    /**
     * The option whose UI callback is running on this thread, or null.
     *
     * Thread-local for the same reason as current_build: the callback is reached from the build, the
     * build is the render thread's, and a nested one - an option's UI running another option's - is
     * still on it. OptionScope is what makes a nested one restore the outer rather than clear it.
     */
    thread_local options::BaseOption* current_option = nullptr;
  }

  ScriptSubmenu::ScriptSubmenu(const std::uint64_t id, std::string name_key, std::string owner_name,
                               std::weak_ptr<as::script::Script> owner, std::string ui_declaration)
      : id_(id),
        key_(ScriptSubmenuIdAllocator::ToRendererKey(id)),
        name_key_(std::move(name_key)),
        owner_name_(std::move(owner_name)),
        owner_(std::move(owner)),
        ui_declaration_(std::move(ui_declaration)) {
  }

  bool ScriptSubmenu::BuildInto(Submenu* frame) {
    if (!frame || IsRemoved()) {
      return false;
    }

    frame_ = frame;

    // Cleared rather than appended to: the frame this is building was cleared by the renderer a moment
    // ago, and what is held here is what that frame's components point at, so a build that kept the
    // last one would hold every component a script ever made for as long as the page exists.
    frame_state_.clear();

    const BuildScope scope(this);
    // Fully qualified: this file is not inside `base::menu::as`, so the relative spelling the other
    // callers use does not reach the binding from here.
    const bool drew = as::bindings::gui::InvokeUiCallback(owner_, ui_declaration_, *this);

    // Null again before this returns: a script that kept the handle it was drawn with and calls Add
    // once its own draw callback has returned is adding to no frame, and AddToFrame says so by doing
    // nothing.
    frame_ = nullptr;
    return drew;
  }

  bool ScriptSubmenu::AddOptionToFrame(options::BaseOption* const option, const std::string& label) {
    if (!option || !frame_) {
      return false;
    }

    // The option's own hook, on the frame a script page is being built into: whatever it draws - which
    // for a script's option is that script's UI callback, and for a built-in is the menu's own drawing
    // of it - arrives in this frame, and every component it draws arrives option-backed.
    option->CreateOptionUi(label, frame_);
    return true;
  }

  ScriptSubmenu* ScriptSubmenu::CurrentBuild() {
    return current_build;
  }

  options::BaseOption* ScriptSubmenu::CurrentOption() {
    return current_option;
  }

  ScriptSubmenu::OptionScope::OptionScope(options::BaseOption* const option) : previous_(current_option) {
    current_option = option;
  }

  ScriptSubmenu::OptionScope::~OptionScope() {
    current_option = previous_;
  }

  ScriptSubmenu::BuildScope::BuildScope(ScriptSubmenu* const sub) : previous_(current_build) {
    current_build = sub;
  }

  ScriptSubmenu::BuildScope::~BuildScope() {
    // What was there before rather than null: the scope is what makes the thread's current page
    // correct again even when the callback it wraps threw, and the value restored is whatever was in
    // force on the way in - nothing here assumes that was nothing.
    current_build = previous_;
  }
}
