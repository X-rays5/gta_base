//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include "../../options/base_option.hpp"
#include "../submenu.hpp"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace base::menu::as::script {
  class Script;
}

namespace base::menu::ui::script_gui {
  /**
   * One page a script draws: the renderer's side of it, and the half the script never sees.
   *
   * A `gui::Submenu` a script holds is a handle over one of these. The page owns the id the renderer
   * knows it by, the key its name is looked up under, the script it belongs to, and the declaration of
   * the function that draws it - everything that has to survive between frames, since the renderer
   * builds the page from nothing every frame.
   *
   * The per-frame part is BuildInto: the renderer holds an update lambda for this page, that lambda
   * calls this, and this runs the script's UI callback, which adds the frame's components by calling
   * back into here. Nothing of the frame is kept between builds, so a script's OnUi says what the page
   * is *now* rather than what changed.
   */
  class ScriptSubmenu final : public std::enable_shared_from_this<ScriptSubmenu> {
  public:
    ScriptSubmenu(std::uint64_t id, std::string name_key, std::string owner_name,
                  std::weak_ptr<as::script::Script> owner, std::string ui_declaration);
    ~ScriptSubmenu() = default;

    ScriptSubmenu(const ScriptSubmenu&) = delete;
    ScriptSubmenu(ScriptSubmenu&&) = delete;
    ScriptSubmenu& operator=(const ScriptSubmenu&) = delete;
    ScriptSubmenu& operator=(ScriptSubmenu&&) = delete;

    [[nodiscard]] std::uint64_t GetId() const {
      return id_;
    }

    /// What the renderer knows this page by, which is the id under a namespace of its own so that it
    /// cannot be confused with an enchantum name - see ScriptSubmenuIdAllocator::ToRendererKey.
    [[nodiscard]] const std::string& GetKey() const {
      return key_;
    }

    /// The key the page's own name is localized by, as a script wrote it: the key it registered, or the
    /// literal it passed, which a miss leaves showing as written.
    [[nodiscard]] const std::string& GetNameKey() const {
      return name_key_;
    }

    /// The name of the script that made this page, which is what unloading that script matches on.
    /// Held as a name rather than read from the owner, because by the time it matters the owner is
    /// being destroyed.
    [[nodiscard]] const std::string& GetOwnerName() const {
      return owner_name_;
    }

    /// The script that made this page, which is resolved again on every build: a page outlives a
    /// reload of the script that drew it only until the reload's cleanup takes it away, and a build
    /// that finds nothing here draws nothing rather than reaching into a dead engine.
    [[nodiscard]] const std::weak_ptr<as::script::Script>& GetOwner() const {
      return owner_;
    }

    /// The declaration of the function that draws this page. A declaration rather than the function:
    /// the engine lends a function to a call and no longer, and a page outlives every call - the same
    /// reason ScriptOption holds its callback that way.
    [[nodiscard]] const std::string& GetUiDeclaration() const {
      return ui_declaration_;
    }

    /**
     * Draws the page into `frame`, which the renderer has already cleared.
     *
     * Runs the script's UI callback inline, on this thread, which is the render thread: a UI callback
     * is a callback and not a tick, so it is not marshalled anywhere and the frame is not built until
     * it has returned. Every component it adds arrives through AddToFrame below.
     *
     * False when the page drew nothing, which is the case for one that has been removed.
     */
    bool BuildInto(Submenu* frame);

    /**
     * Appends a component to the frame being built, and holds `keepalive` for it.
     *
     * `keepalive` is whatever the component's C++ side points at: a ToggleComponent holds a raw
     * `std::atomic<bool>*`, and the shared_ptr it came from is what keeps that alive. A component a
     * script made inline - `sub.Add(gui::Toggle(...))` - is a temporary the engine destroys at the end
     * of the statement, so without the keepalive the frame would be reading freed state by the time it
     * is drawn. A component a script keeps a handle to owns its state anyway, and the keepalive is then
     * a second reference to the same thing.
     *
     * The keepalive-less overload is for the kinds that own their own state - Label, List, Range,
     * Execute, Link - where there is nothing to hold and allocating a keepalive per component per frame
     * would be a heap allocation for nothing.
     *
     * The component is option-backed when a script option's own UI callback is what is running; see
     * CurrentOption. That is what gives it the hotkey and the saving a C++ option's components have.
     */
    template <typename Keep, typename T>
    void AddToFrame(Keep&& keepalive, T&& component) {
      if (!frame_) {
        return;
      }

      Draw(std::forward<T>(component));
      frame_state_.emplace_back(std::make_shared<std::decay_t<Keep>>(std::forward<Keep>(keepalive)));
    }

    /// The same, for a component whose C++ side owns everything it points at.
    template <typename T>
    void AddToFrame(T&& component) {
      Draw(std::forward<T>(component));
    }

    /**
     * Draws `option`'s own UI into the frame being built, by running the hook it implements - which for
     * an option a script registered runs that script's UI callback, and for one of the menu's own draws
     * whatever the menu draws for it.
     *
     * This is what makes "the option registry's create-ui function" reachable from a script: a page
     * draws a built-in option's row, or its own option's row, without knowing what either looks like.
     *
     * False when no frame is being built, or when the option has nothing to draw.
     */
    bool AddOptionToFrame(options::BaseOption* option, const std::string& label);

    /**
     * The option whose UI callback is running on this thread, or null.
     *
     * Set by OptionScope around that callback, and read by everything a script adds from inside it: a
     * component a script draws for its own option is that option's component, exactly as one a C++ body
     * draws with GTA_BASE_OPTION_UI_COMP is.
     */
    [[nodiscard]] static options::BaseOption* CurrentOption();

    /**
     * Makes `option` the thread's current one for as long as it lives, which is the length of one run of
     * its UI callback. An RAII scope rather than a pair of calls so that a callback which throws still
     * puts the previous one back - the same reason BuildScope is one.
     */
    class OptionScope {
    public:
      explicit OptionScope(options::BaseOption* option);
      ~OptionScope();

      OptionScope(const OptionScope&) = delete;
      OptionScope& operator=(const OptionScope&) = delete;

    private:
      options::BaseOption* previous_;
    };

    /// Marks the page as gone. The renderer's copy is what keeps it alive, so this is a note that the
    /// next build - if one is somehow still reached - draws nothing, rather than a teardown.
    void SetRemoved() {
      removed_.store(true, std::memory_order_relaxed);
    }

    [[nodiscard]] bool IsRemoved() const {
      return removed_.load(std::memory_order_relaxed);
    }

    /**
     * The page whose frame is being built on this thread, or null.
     *
     * This is how a component a script adds inside an option's UI callback finds the page it belongs
     * to: the build is already under way on this thread, and the option's UI callback is called from
     * inside it, so the page being built is the one to add to.
     */
    [[nodiscard]] static ScriptSubmenu* CurrentBuild();

  private:
    /// Appends `component` to the frame being built, option-backed when there is a current option. Does
    /// nothing when there is no frame, which is a script adding outside a build.
    template <typename T>
    void Draw(T&& component) {
      if (!frame_) {
        return;
      }

      if (options::BaseOption* const option = CurrentOption()) {
        frame_->AddComponent(option, std::forward<T>(component));
      } else {
        frame_->AddComponent(std::forward<T>(component));
      }
    }

    /// The thread's current build, set and cleared by BuildInto. Kept as its own scope object so that
    /// a callback which throws still puts the previous one back.
    class BuildScope {
    public:
      explicit BuildScope(ScriptSubmenu* sub);
      ~BuildScope();

      BuildScope(const BuildScope&) = delete;
      BuildScope& operator=(const BuildScope&) = delete;

    private:
      ScriptSubmenu* previous_;
    };

    std::uint64_t id_;
    std::string key_;
    std::string name_key_;
    std::string owner_name_;
    std::weak_ptr<as::script::Script> owner_;
    std::string ui_declaration_;
    std::atomic<bool> removed_{false};

    /**
     * The frame `BuildInto` is filling, and what it is filling it with. Both are this thread's - the
     * render thread's - for the length of the build, and `frame_` is null again once it returns: a
     * script that kept its `gui::Submenu@` and calls Add after its own draw callback has returned is
     * adding to nothing, which is the honest answer, since there is no frame for it to be part of.
     */
    Submenu* frame_{nullptr};
    std::vector<std::shared_ptr<void>> frame_state_;
  };
}

namespace base::menu::as::bindings::gui {
  /**
   * Runs `owner`'s `declaration` with a handle to `sub` as its only argument, on this thread.
   *
   * Declared here and defined in as/bindings/as_gui.cpp, which is where the type a `gui::Submenu@`
   * names lives: a page knows the script it belongs to and the function that draws it, and nothing
   * about the engine's types. False when the script is gone, the function is not in the module, or the
   * call failed.
   */
  bool InvokeUiCallback(const std::weak_ptr<as::script::Script>& owner, const std::string& declaration,
                        ui::script_gui::ScriptSubmenu& sub);
}
