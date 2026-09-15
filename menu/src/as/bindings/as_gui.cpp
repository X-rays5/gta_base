//
// Created by X-ray on 12/09/2026.
//

#include "as_gui.hpp"

#include "as_option.hpp"
#include "../script/as_script.hpp"
#include "../script/as_script_context.hpp"
#include "../script/as_script_manager.hpp"
#include "../util/as_bind.hpp"
#include "../../ui/components/components.hpp"
#include "../../ui/localization/manager.hpp"
#include "../../ui/script_gui/script_submenu.hpp"
#include "../../ui/script_gui/script_submenu_registry.hpp"

#include <angelscript/scriptarray/scriptarray.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// A script's page is the menu's own page seen from a script, and this file is the whole of the layer
// between the two: the renderer, the submenus and the components know nothing about scripts, and the
// classes below know nothing about the renderer.
//
// Three things about that layer are worth knowing before reading it.
//
// The first is what a component is. There is one `gui::Component` base and one class per kind of
// component, each owning the state its C++ counterpart points at and exposing the part of that state a
// script is allowed to touch. A kind is registered as its own type - `gui::Toggle`, `gui::List` - and
// converted to the base by an `opImplCast` object method, since AngelScript 2.38 has no base-type
// parameter on RegisterObjectType and no cast behaviours. That is the idiom the vendored SDK uses for
// weakref, and it is what gives `Add` one parameter type for the life of the binding: a new component
// kind is a new class and one more registration, and nothing that already exists changes.
//
// The second is when a script may add anything. Only while a page is being drawn: `Add` and `AddOption`
// push into the frame `ScriptSubmenu::BuildInto` is filling, and a script that calls one once its own draw
// callback has returned is adding to nothing, which is the honest answer rather than a crash or a deferred
// addition nobody would ever see drawn.
//
// The third is how a callback is reached. A callback is held as its declaration plus a weak handle to
// the script that declared it, resolved again for every call - the engine lends a function to a native
// for the length of that call and no longer, so a page that outlives every call can hold nothing else.
// The GUI callback is called with the *address of a handle variable* rather than with the handle, which
// is the same shape the option binding uses and for the same reason: see ScriptOptionHandle.
//
// Every handle parameter is declared with `@+`, which is what makes the engine release the reference it
// was given once the call returns - see the SDK's doc_obj_handle.h. Without it the native would own that
// reference, and a call that did not release it would leak one per call on a path that runs every frame.
namespace base::menu::as::bindings::gui {
  namespace {
    // The three namespaces below live beside the GUI rather than under these bindings, so they are
    // reached by alias rather than by relative lookup.
    namespace components = ui::components;
    namespace localization = ui::localization;
    namespace script_gui = ui::script_gui;

    using menu::options::BaseOption;
    using script::Script;
    using script::ScriptContext;
    using script::ScriptManager;
    using script_gui::ScriptSubmenu;

    /// Every page handle that exists, counted by the two ends of its life - see SubmenuHandle.
    std::atomic<std::int64_t> live_handles{0};

    /// The script running right now, by name and by handle, which are the same fact: the name is what
    /// the registry is told to unload a script's pages by, and the handle is what resolves a callback
    /// again inside the script that declared it.
    std::weak_ptr<Script> RunningScript(std::string& name) {
      name = ScriptManager::RunningScriptName();
      if (name.empty() || !script::kAS_SCRIPT_MANAGER) {
        return {};
      }

      if (const auto found = script::kAS_SCRIPT_MANAGER->GetScript(name); !found.has_error()) {
        return found.value();
      }

      return {};
    }

    /// The same script, for a caller that wants the script itself rather than what it is called.
    std::shared_ptr<Script> RunningScript() {
      std::string name;
      return RunningScript(name).lock();
    }

    /// Whether `fn` is the shape a UI callback is called with: `void fn(Submenu@ &in sub)`.
    ///
    /// The funcdef parameter is what really enforces this - a function of any other shape is not a
    /// `gui::UiCallback` and the compiler refuses to convert it - so this is the second lock on the same
    /// door, and the place the report names the shape a script got wrong.
    bool IsUiCallbackShape(AngelScript::asIScriptFunction* fn) {
      if (fn->GetReturnTypeId() != AngelScript::asTYPEID_VOID || fn->GetParamCount() != 1) {
        return false;
      }

      int type_id = 0;
      AngelScript::asDWORD flags = 0;
      if (fn->GetParam(0, &type_id, &flags, nullptr, nullptr) < 0) {
        return false;
      }

      // A handle, and one the callee is handed as a reference: that pair is what makes the argument the
      // caller's to hold rather than the engine's to count, and the reason the address of a handle
      // variable is what the call is given.
      return (type_id & AngelScript::asTYPEID_OBJHANDLE) != 0 &&
             (flags & AngelScript::asTM_INOUTREF) == AngelScript::asTM_INREF;
    }

    /// The declaration a callback is held by, which is the function itself in everything but name: the
    /// engine lends a function to the native it is passed to for the length of that call and no longer,
    /// and a component or a page outlives every call that made it. Namespaces included, so that a
    /// function of one is found again from its declaration alone.
    std::string CallbackDeclaration(AngelScript::asIScriptFunction* fn) {
      return fn ? fn->GetDeclaration(true, true) : std::string{};
    }

    /**
     * Runs `declaration` in `owner`'s module, on this thread. False when the script is gone, the
     * function it named is not in the module, or the call threw.
     *
     * A callback with no argument, which is what an action - an Execute row, a Link's on-open - is. Any
     * thread may call into a script: only the tick functions are reserved for a thread of their own, and
     * this runs wherever the menu's own handler for the component ran.
     */
    bool InvokeActionCallback(const std::weak_ptr<Script>& owner, const std::string& declaration) {
      if (declaration.empty()) {
        return false;
      }

      const std::shared_ptr<Script> script = owner.lock();
      if (!script) {
        return false;
      }

      auto* const fn = script->GetFunctionByDecl(declaration);
      if (!fn) {
        LOG_ERROR("[AS] The callback '{}' cannot be found in the script it was declared in", declaration);
        return false;
      }

      try {
        ScriptContext context(fn->GetEngine());

        // Called, not started as a coroutine: this is a button press, and whoever pressed it is waiting
        // for the menu to answer. A callback that waits therefore waits here, on the thread the menu's
        // own handler would have waited on.
        context.Run(fn, false);
      } catch (const std::exception& e) {
        LOG_ERROR("[AS] The callback '{}' threw: {}", declaration, e.what());
        return false;
      }

      return true;
    }

    // -------------------------------------------------------------------------------------------------
    // The component hierarchy
    // -------------------------------------------------------------------------------------------------

    /**
     * What a script holds any component by: `gui::Component` is registered as a reference type with no
     * methods of its own, so that `Add` has one parameter type forever and a script can keep a Label and
     * a Toggle in the same kind of variable.
     *
     * A component is a description of a row rather than a row: a script builds one - `gui::Toggle(...)`
     * - and hands it to `Add`, which turns it into the menu's own component for the frame being drawn.
     * That is why a component can be made and thrown away without anything being drawn, and why the
     * state a script reads back - a Toggle's bool, a List's index - is held here rather than in the
     * frame's component: the frame is rebuilt every frame and the state outlives all of them.
     */
    class ScriptComponent {
    public:
      ScriptComponent() = default;
      virtual ~ScriptComponent() = default;
      ScriptComponent(const ScriptComponent&) = delete;
      ScriptComponent(ScriptComponent&&) = delete;
      ScriptComponent& operator=(const ScriptComponent&) = delete;
      ScriptComponent& operator=(ScriptComponent&&) = delete;

      /// Draws this into `sub`'s frame, which is the frame being built right now.
      virtual void AddTo(ScriptSubmenu& sub) const = 0;

      /// AngelScript's reference count: a component is a reference type, and the engine counts it while
      /// a script holds it or passes it to Add. Mutable so that the conversion to the base can be an
      /// `opImplCast` declared const, as the SDK declares its own.
      mutable std::atomic<std::uint32_t> refs_{1};
    };

    void ComponentAddRef(ScriptComponent* self) {
      self->refs_.fetch_add(1, std::memory_order_relaxed);
    }

    void ComponentRelease(ScriptComponent* self) {
      if (self->refs_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete self;
      }
    }

    /// The derived-to-base conversion, registered on every component kind as `Component@ opImplCast()
    /// const`. AngelScript 2.38 has no cast behaviours, so a method of that name returning a handle is
    /// what makes a `gui::Toggle@` satisfy `Add(gui::Component@)` by an implicit conversion - the idiom
    /// the vendored SDK's weakref uses. A reference is taken for the returned handle, which is what the
    /// engine counts for a plain `@` return; a script never writes `cast<>` here, which these types do
    /// not support.
    ScriptComponent* CastToComponent(const ScriptComponent* self) {
      self->refs_.fetch_add(1, std::memory_order_relaxed);
      return const_cast<ScriptComponent*>(self);
    }

    /// A row of centred, non-selectable text. Nothing of it is state a script reads back, so the handle
    /// owns nothing and the frame's component is the whole of it.
    class ScriptLabel final : public ScriptComponent {
    public:
      explicit ScriptLabel(std::string text) : text_(std::move(text)) {}

      void AddTo(ScriptSubmenu& sub) const override {
        sub.AddToFrame(components::LabelComponent(text_));
      }

    private:
      std::string text_;
    };

    /// A checkbox: a name, a description, and the bool it sets.
    ///
    /// The bool is a shared_ptr rather than a member because the frame's ToggleComponent points at it:
    /// a script that writes `sub.Add(gui::Toggle("enabled", "", false))` makes a temporary the engine
    /// destroys at the end of the statement, and the frame outlives it. AddToFrame's keepalive is what
    /// holds this shared_ptr for as long as the frame's component points at it, and a script that keeps
    /// the handle instead holds its own reference to the same bool.
    class ScriptToggle final : public ScriptComponent {
    public:
      ScriptToggle(std::string name, std::string description, const bool value)
          : name_(std::move(name)), description_(std::move(description)), state_(std::make_shared<std::atomic<bool>>(value)) {}

      void AddTo(ScriptSubmenu& sub) const override {
        sub.AddToFrame(state_, components::ToggleComponent(name_, description_, state_.get()));
      }

      [[nodiscard]] bool Get() const {
        return state_->load();
      }

      void Set(const bool value) {
        state_->store(value);
      }

    private:
      std::string name_;
      std::string description_;
      std::shared_ptr<std::atomic<bool>> state_;
    };

    /// One item of a list, which is the only shape a script's list can be built from: the menu's lists
    /// are lists of localized strings, so an item is a key or a literal like every other text.
    std::string ArrayElementAt(AngelScript::CScriptArray* const array, const AngelScript::asUINT index) {
      if (!array || index >= array->GetSize()) {
        return {};
      }

      const auto* const element = static_cast<const std::string*>(array->At(index));
      return element ? *element : std::string{};
    }

    /// A list of items, with or without a checkbox on the same row.
    ///
    /// One class for both kinds and two AngelScript types over it: `gui::List` and `gui::ToggleList` are
    /// the same row apart from whether there is a bool beside it, and the bool is what the class holds
    /// or does not. The state a script reads is the index and, for a toggle list, the checkbox; both are
    /// shared with the frame's component, which owns them - so an inline list survives the statement
    /// that made it with no keepalive at all.
    class ScriptList final : public ScriptComponent {
    public:
      ScriptList(std::string name, std::string description, AngelScript::CScriptArray* const items, const std::int32_t index,
                 const bool togglable, const bool toggled)
          : name_(std::move(name)),
            description_(std::move(description)),
            items_(TakeItems(items)),
            index_(std::make_shared<std::atomic<std::size_t>>(0)) {
        if (togglable) {
          toggled_ = std::make_shared<std::atomic<bool>>(toggled);
        }

        index_->store(ClampIndex(index));
      }

      void AddTo(ScriptSubmenu& sub) const override {
        if (toggled_) {
          sub.AddToFrame(components::ToggleListComponent(name_, description_, items_, index_, toggled_));
        } else {
          sub.AddToFrame(components::ListComponent(name_, description_, items_, index_));
        }
      }

      [[nodiscard]] std::int32_t Get() const {
        return static_cast<std::int32_t>(index_->load());
      }

      void Set(const std::int32_t index) {
        index_->store(ClampIndex(index));
      }

      [[nodiscard]] std::int32_t GetItemCount() const {
        return static_cast<std::int32_t>(items_->size());
      }

      [[nodiscard]] std::string GetItem(const std::int32_t index) const {
        if (index < 0 || static_cast<std::size_t>(index) >= items_->size()) {
          LOG_ERROR("[AS] The list '{}' was asked for item {}, which it does not have: it has {} items",
                    name_, index, items_->size());
          return {};
        }

        return (*items_)[static_cast<std::size_t>(index)];
      }

      /// Replaces every item, by publishing a new list rather than by changing the one the frame is
      /// reading: the render thread reads the snapshot it was built with while this builds the next one,
      /// and the swap is one pointer.
      void SetItems(AngelScript::CScriptArray* const items) {
        items_ = TakeItems(items);

        // Clamped rather than reset, so that a list a script rebuilds every tick keeps where the player
        // had got to - which is the whole reason to rebuild it rather than replace it.
        const std::size_t current = index_->load();
        index_->store(items_->empty() ? 0 : std::min(current, items_->size() - 1));
      }

      [[nodiscard]] bool GetToggled() const {
        return toggled_ && toggled_->load();
      }

      void SetToggled(const bool toggled) {
        if (toggled_) {
          toggled_->store(toggled);
        }
      }

    private:
      /// The array as the snapshot the frame's component reads. A list a script never touches is copied
      /// once; one it rebuilds every tick is copied once per rebuild, which is the price of the render
      /// thread reading it without a lock.
      static std::shared_ptr<const std::vector<std::string>> TakeItems(AngelScript::CScriptArray* const items) {
        auto taken = std::make_shared<std::vector<std::string>>();
        if (!items) {
          return taken;
        }

        taken->reserve(items->GetSize());
        for (AngelScript::asUINT i = 0; i < items->GetSize(); ++i) {
          taken->push_back(ArrayElementAt(items, i));
        }

        return taken;
      }

      /// `index` as an index this list has: zero for an empty list or one before the start, and the last
      /// item for one past the end.
      [[nodiscard]] std::size_t ClampIndex(const std::int32_t index) const {
        if (items_->empty() || index < 0) {
          return 0;
        }

        return std::min(static_cast<std::size_t>(index), items_->size() - 1);
      }

      std::string name_;
      std::string description_;
      std::shared_ptr<const std::vector<std::string>> items_;
      std::shared_ptr<std::atomic<std::size_t>> index_;

      /// The checkbox, for a `gui::ToggleList`, and null for a `gui::List`. Which of the two this is, is
      /// decided once, when it is made, and never changes.
      std::shared_ptr<std::atomic<bool>> toggled_;
    };

    /// The two halves a toggle range's component points at, held together so that one keepalive covers
    /// both: the component holds a raw pointer to each and neither is its own.
    template <typename T>
    struct ToggleRangeState {
      std::shared_ptr<std::atomic<T>> value;
      std::shared_ptr<std::atomic<bool>> toggled;
    };

    /// A number range, with or without a checkbox on the same row, as one class over the four
    /// AngelScript types `gui::IntRange`, `gui::FloatRange`, `gui::ToggleIntRange` and
    /// `gui::ToggleFloatRange` - the same split the menu makes, in the same shape as ScriptList's.
    template <typename T>
    class ScriptRange final : public ScriptComponent {
    public:
      ScriptRange(std::string name, std::string description, const T value, const T min, const T max, const T step,
                  const bool togglable, const bool toggled)
          : name_(std::move(name)),
            description_(std::move(description)),
            value_(std::make_shared<std::atomic<T>>(value)),
            min_(min),
            max_(max),
            step_(step) {
        if (togglable) {
          toggled_ = std::make_shared<std::atomic<bool>>(toggled);
        }
      }

      void AddTo(ScriptSubmenu& sub) const override {
        if (toggled_) {
          sub.AddToFrame(
              ToggleRangeState<T>{value_, toggled_},
              components::ToggleNumberRangeComponent<T>(name_, description_, value_.get(), min_, max_, step_, toggled_.get()));
        } else {
          // The component takes the shared_ptr rather than the pointer, so this one needs no keepalive.
          sub.AddToFrame(components::NumberRangeComponent<T>(name_, description_, value_, min_, max_, step_));
        }
      }

      [[nodiscard]] T Get() const {
        return value_->load();
      }

      void Set(const T value) {
        value_->store(value);
      }

      [[nodiscard]] T GetMin() const {
        return min_;
      }

      [[nodiscard]] T GetMax() const {
        return max_;
      }

      [[nodiscard]] T GetStep() const {
        return step_;
      }

      [[nodiscard]] bool GetToggled() const {
        return toggled_ && toggled_->load();
      }

      void SetToggled(const bool toggled) {
        if (toggled_) {
          toggled_->store(toggled);
        }
      }

    private:
      std::string name_;
      std::string description_;
      std::shared_ptr<std::atomic<T>> value_;
      T min_;
      T max_;
      T step_;
      std::shared_ptr<std::atomic<bool>> toggled_;
    };

    /// A row that runs something when it is pressed. The handler runs where a C++ one runs - on the
    /// menu's thread pool - and calls back into the script from there, which is what "any thread may
    /// call into a script" means in practice.
    class ScriptExecute final : public ScriptComponent {
    public:
      ScriptExecute(std::string name, std::string description, AngelScript::asIScriptFunction* const handler)
          : name_(std::move(name)),
            description_(std::move(description)),
            owner_(RunningScript()),
            declaration_(CallbackDeclaration(handler)) {}

      void AddTo(ScriptSubmenu& sub) const override {
        const std::weak_ptr<Script> owner = owner_;
        const std::string declaration = declaration_;

        sub.AddToFrame(components::ExecuteComponent<std::function<void()>>(name_, description_, [owner, declaration] {
          static_cast<void>(InvokeActionCallback(owner, declaration));
        }));
      }

    private:
      std::string name_;
      std::string description_;
      std::weak_ptr<Script> owner_;
      std::string declaration_;
    };

    /// A row that goes to another page, running something on the way - the same navigation the menu's
    /// own links do, which does not wait for the handler.
    class ScriptLink final : public ScriptComponent {
    public:
      ScriptLink(std::string name, std::string description, std::shared_ptr<ScriptSubmenu> target,
                 AngelScript::asIScriptFunction* const handler)
          : name_(std::move(name)),
            description_(std::move(description)),
            target_(std::move(target)),
            owner_(RunningScript()),
            declaration_(CallbackDeclaration(handler)) {}

      void AddTo(ScriptSubmenu& sub) const override {
        std::function<void()> handler;
        if (!declaration_.empty()) {
          const std::weak_ptr<Script> owner = owner_;
          const std::string declaration = declaration_;
          handler = [owner, declaration] {
            static_cast<void>(InvokeActionCallback(owner, declaration));
          };
        }

        // The renderer-free form of the row, which is the one a link to a script's page needs: the
        // target is a key the registry made, not one of the renderer's ids, so there is nothing to
        // resolve. The component allows an empty handler, which is what a link with no on-open is.
        auto component = components::SubLinkComponent<std::function<void()>>(target_->GetKey(), name_, std::move(handler));
        component.SetDescription(description_);
        sub.AddToFrame(std::move(component));
      }

    private:
      std::string name_;
      std::string description_;
      std::shared_ptr<ScriptSubmenu> target_;
      std::weak_ptr<Script> owner_;
      std::string declaration_;
    };

    // -------------------------------------------------------------------------------------------------
    // The page handle
    // -------------------------------------------------------------------------------------------------

    /**
     * The script-facing `Submenu`: what a script holds while it builds a page out of one, and what its UI
     * callback is given.
     *
     * It holds the page strongly, so a handle a script kept from its OnUi call - `g_root` - can never
     * name a freed page. The page itself is what goes inert when its script unloads: the registry removes
     * it, and a handle to a removed page reports and does nothing rather than drawing into a frame
     * nothing will read.
     */
    class SubmenuHandle {
    public:
      explicit SubmenuHandle(std::shared_ptr<ScriptSubmenu> sub) : sub_(std::move(sub)) {
        live_handles.fetch_add(1, std::memory_order_relaxed);
      }

      /// AngelScript's reference count - see ScriptOptionHandle, which is the same shape.
      void AddRef() {
        refs_.fetch_add(1, std::memory_order_relaxed);
      }

      void Release() {
        if (refs_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
          live_handles.fetch_sub(1, std::memory_order_relaxed);
          delete this;
        }
      }

      [[nodiscard]] std::uint64_t GetId() const {
        return sub_ ? sub_->GetId() : 0;
      }

      [[nodiscard]] std::string GetName() const {
        return sub_ ? sub_->GetNameKey() : std::string{};
      }

      [[nodiscard]] const std::shared_ptr<ScriptSubmenu>& GetSubmenu() const {
        return sub_;
      }

      /// `Add(component)` - draws the component into the page being built right now.
      ///
      /// The page being built is the one `BuildInto` is filling on this thread, which for a script's own
      /// UI callback is the page it was handed. Anywhere else - after the callback returned, or in a
      /// task - there is no frame to draw into, and this says so rather than deferring the row to a
      /// frame that would never show it.
      [[nodiscard]] bool Add(const ScriptComponent* const component) const {
        if (!component) {
          LOG_ERROR("[AS] Add was given no component, so there is nothing to draw");
          return false;
        }

        if (ScriptSubmenu* const page = ScriptSubmenu::CurrentBuild()) {
          component->AddTo(*page);
          return true;
        }

        LOG_ERROR("[AS] A component was added to '{}' outside of a page being drawn: a page is built by the "
                  "UI callback it was made with, and a component added anywhere else - once that callback has "
                  "returned, say - is added to nothing", GetName());
        return false;
      }

      /// `AddOption(option, label)` - draws an option's own row into the page being built right now.
      ///
      /// The option draws itself: for one of the menu's own that is whatever the menu draws for it, and
      /// for one a script registered it is that option's UI callback. Either way a script's page holds a
      /// row that behaves exactly as the option's row does everywhere else, and the `label` is passed to
      /// the option whose row it is rather than being drawn here.
      [[nodiscard]] bool AddOption(const option::ScriptOptionHandle* const option, const std::string& label) const {
        if (!option) {
          LOG_ERROR("[AS] AddOption was given no option, so there is nothing to draw");
          return false;
        }

        if (ScriptSubmenu* const page = ScriptSubmenu::CurrentBuild()) {
          return page->AddOptionToFrame(option->GetOption().get(), label);
        }

        LOG_ERROR("[AS] An option was added to '{}' outside of a page being drawn - see Add", GetName());
        return false;
      }

      [[nodiscard]] bool RegisterRoot() const {
        if (!script_gui::kSCRIPT_SUBMENUS) {
          LOG_ERROR("[AS] '{}' was registered outside of the menu, which has no pages registered", GetName());
          return false;
        }

        return script_gui::kSCRIPT_SUBMENUS->RegisterRoot(sub_);
      }

      [[nodiscard]] bool Remove() const {
        if (!script_gui::kSCRIPT_SUBMENUS) {
          return false;
        }

        return script_gui::kSCRIPT_SUBMENUS->Remove(sub_);
      }

      /// Takes away another page of the same script, which is how a script that makes pages as it goes
      /// drops one without dropping all of them. A page that is not this script's - or this one itself -
      /// is refused, so that RemoveChild is never Remove with an extra step.
      [[nodiscard]] bool RemoveChild(const SubmenuHandle* const page) const {
        const std::shared_ptr<ScriptSubmenu> other = page ? page->GetSubmenu() : nullptr;
        if (!script_gui::kSCRIPT_SUBMENUS || !other || other == sub_) {
          return false;
        }

        if (!sub_ || other->GetOwnerName() != sub_->GetOwnerName()) {
          LOG_ERROR("[AS] '{}' cannot remove a page of another script", GetName());
          return false;
        }

        return script_gui::kSCRIPT_SUBMENUS->Remove(other);
      }

      /// How many handles exist right now, which is how a test holds the engine to releasing the ones it
      /// made - one per page a script made and one per UI callback it was drawn with.
      [[nodiscard]] static std::int64_t LiveInstances() {
        return live_handles.load(std::memory_order_relaxed);
      }

    private:
      std::atomic<std::uint32_t> refs_{1};
      std::shared_ptr<ScriptSubmenu> sub_;
    };

    // -------------------------------------------------------------------------------------------------
    // The factories
    // -------------------------------------------------------------------------------------------------

    ScriptLabel* CreateLabel(const std::string& text) {
      return new ScriptLabel(text);
    }

    ScriptToggle* CreateToggle(const std::string& name, const std::string& description, const bool value) {
      return new ScriptToggle(name, description, value);
    }

    ScriptList* CreateList(const std::string& name, const std::string& description, AngelScript::CScriptArray* const items,
                           const std::int32_t index) {
      return new ScriptList(name, description, items, index, false, false);
    }

    ScriptList* CreateToggleList(const std::string& name, const std::string& description, AngelScript::CScriptArray* const items,
                                 const std::int32_t index, const bool toggled) {
      return new ScriptList(name, description, items, index, true, toggled);
    }

    template <typename T>
    ScriptRange<T>* CreateRange(const std::string& name, const std::string& description, const T value, const T min,
                                const T max, const T step) {
      return new ScriptRange<T>(name, description, value, min, max, step, false, false);
    }

    template <typename T>
    ScriptRange<T>* CreateToggleRange(const std::string& name, const std::string& description, const T value, const T min,
                                      const T max, const T step, const bool toggled) {
      return new ScriptRange<T>(name, description, value, min, max, step, true, toggled);
    }

    ScriptExecute* CreateExecute(const std::string& name, const std::string& description,
                                 AngelScript::asIScriptFunction* const handler) {
      if (!handler) {
        LOG_ERROR("[AS] '{}' was made without a callback, so pressing it would do nothing", name);
        return nullptr;
      }

      return new ScriptExecute(name, description, handler);
    }

    ScriptLink* CreateLink(const std::string& name, const std::string& description, const SubmenuHandle* const target,
                           AngelScript::asIScriptFunction* const handler) {
      if (!target || !target->GetSubmenu()) {
        LOG_ERROR("[AS] '{}' was made without a page to go to, so pressing it would do nothing", name);
        return nullptr;
      }

      return new ScriptLink(name, description, target->GetSubmenu(), handler);
    }

    /// `Submenu(name, ui)` - a page owned by the script that made it and drawn by `ui`.
    ///
    /// Every page is registered with the renderer here, so that a link to one has somewhere to go; which
    /// of them is *listed* on the root page of the menu is RegisterRoot's business, and it lists one per
    /// script at most.
    SubmenuHandle* CreateSubmenuWithUi(const std::string& name_key, AngelScript::asIScriptFunction* const ui) {
      if (!script_gui::kSCRIPT_SUBMENUS) {
        LOG_ERROR("[AS] A page was made outside of the menu, which has nowhere to put it");
        return nullptr;
      }

      std::string owner_name;
      const std::weak_ptr<Script> owner = RunningScript(owner_name);
      if (owner_name.empty()) {
        LOG_ERROR("[AS] A page was made outside of a loaded script, so there is nothing to draw it and nothing "
                  "to remove it when that script goes");
        return nullptr;
      }

      std::string declaration;
      if (ui) {
        if (!IsUiCallbackShape(ui)) {
          LOG_ERROR("[AS] A page was made with '{}' as its UI callback, which is not the shape a UI callback is "
                    "called with: it takes one parameter, `gui::Submenu@ &in sub`", ui->GetDeclaration());
          return nullptr;
        }

        declaration = CallbackDeclaration(ui);
      }

      return new SubmenuHandle(script_gui::kSCRIPT_SUBMENUS->Create(name_key, owner_name, owner, std::move(declaration)));
    }

    /// `Submenu(name)` - the same page with nothing to draw it. Only useful for one that is registered so
    /// that something else can list it, since a page's own rows are drawn by the callback it was made with.
    SubmenuHandle* CreateSubmenuWithoutUi(const std::string& name_key) {
      return CreateSubmenuWithUi(name_key, nullptr);
    }

    // -------------------------------------------------------------------------------------------------
    // The thunks
    // -------------------------------------------------------------------------------------------------

    void SubmenuAddRef(SubmenuHandle* const self) {
      self->AddRef();
    }

    void SubmenuRelease(SubmenuHandle* const self) {
      self->Release();
    }

    std::uint64_t SubmenuGetId(const SubmenuHandle* const self) {
      return self->GetId();
    }

    std::string SubmenuGetName(const SubmenuHandle* const self) {
      return self->GetName();
    }

    bool SubmenuAdd(const SubmenuHandle* const self, const ScriptComponent* const component) {
      return self->Add(component);
    }

    bool SubmenuAddOption(const SubmenuHandle* const self, const option::ScriptOptionHandle* const opt,
                          const std::string& label) {
      return self->AddOption(opt, label);
    }

    bool SubmenuRegisterRoot(const SubmenuHandle* const self) {
      return self->RegisterRoot();
    }

    bool SubmenuRemove(const SubmenuHandle* const self) {
      return self->Remove();
    }

    bool SubmenuRemoveChild(const SubmenuHandle* const self, const SubmenuHandle* const page) {
      return self->RemoveChild(page);
    }

    // The mutations a script may do to a component it holds, each a plain call onto the class. A
    // component's state is its own, so none of this needs to know which page it is on or whether one is
    // being drawn - which is what makes `g_enabled.Set(true)` from a task mean what it says.

    bool ToggleGet(const ScriptToggle* const self) {
      return self->Get();
    }

    void ToggleSet(ScriptToggle* const self, const bool value) {
      self->Set(value);
    }

    std::int32_t ListGet(const ScriptList* const self) {
      return self->Get();
    }

    void ListSet(ScriptList* const self, const std::int32_t index) {
      self->Set(index);
    }

    std::int32_t ListGetItemCount(const ScriptList* const self) {
      return self->GetItemCount();
    }

    std::string ListGetItem(const ScriptList* const self, const std::int32_t index) {
      return self->GetItem(index);
    }

    void ListSetItems(ScriptList* const self, AngelScript::CScriptArray* const items) {
      self->SetItems(items);
    }

    bool ListGetToggled(const ScriptList* const self) {
      return self->GetToggled();
    }

    void ListSetToggled(ScriptList* const self, const bool toggled) {
      self->SetToggled(toggled);
    }

    template <typename T>
    T RangeGet(const ScriptRange<T>* const self) {
      return self->Get();
    }

    template <typename T>
    void RangeSet(ScriptRange<T>* const self, const T value) {
      self->Set(value);
    }

    template <typename T>
    T RangeGetMin(const ScriptRange<T>* const self) {
      return self->GetMin();
    }

    template <typename T>
    T RangeGetMax(const ScriptRange<T>* const self) {
      return self->GetMax();
    }

    template <typename T>
    T RangeGetStep(const ScriptRange<T>* const self) {
      return self->GetStep();
    }

    template <typename T>
    bool RangeGetToggled(const ScriptRange<T>* const self) {
      return self->GetToggled();
    }

    template <typename T>
    void RangeSetToggled(ScriptRange<T>* const self, const bool toggled) {
      self->SetToggled(toggled);
    }

    // -------------------------------------------------------------------------------------------------
    // Translations
    // -------------------------------------------------------------------------------------------------

    /// The namespace a script's translation keys live in: the same prefix its options are registered
    /// under, so that two scripts that both register "menu" cannot answer for each other.
    std::string TranslationPrefix(const std::shared_ptr<Script>& script) {
      return script ? script->GetOptionPrefix() : std::string{};
    }

    /// The key a script's string is stored under. A script writes the short name - `"spawn"` - and the
    /// full key - `"optreg/spawn"` - is what the localization manager sees, which is what makes a key the
    /// menu itself uses unreachable from a script.
    std::string QualifyKey(const std::string& prefix, const std::string& key) {
      if (prefix.empty()) {
        return key;
      }

      return std::format("{}/{}", prefix, key);
    }

    /// `translations::Register(key, value)` - one string, owned by the running script.
    bool RegisterTranslation(const std::string& key, const std::string& value) {
      if (!localization::kMANAGER) {
        LOG_ERROR("[AS] A translation was registered outside of the menu, which has no translations");
        return false;
      }

      std::string owner_name;
      const std::shared_ptr<Script> script = RunningScript(owner_name).lock();
      if (!script) {
        LOG_ERROR("[AS] The translation '{}' was registered outside of a loaded script, so there would be "
                  "nothing to remove it when that script goes", key);
        return false;
      }

      return localization::kMANAGER->RegisterScriptTranslation(QualifyKey(TranslationPrefix(script), key), value, owner_name);
    }

    /// `translations::Remove(key)` - the same string, taken away again, which is what a script that
    /// changes its own text does rather than waiting for its unload to do it.
    bool RemoveTranslation(const std::string& key) {
      if (!localization::kMANAGER) {
        return false;
      }

      std::string owner_name;
      const std::shared_ptr<Script> script = RunningScript(owner_name).lock();
      if (!script) {
        return false;
      }

      return localization::kMANAGER->RemoveScriptTranslation(QualifyKey(TranslationPrefix(script), key), owner_name);
    }

    /// `translations::Key(key)` - the short name a script writes, as the key the menu resolves. This is
    /// what a script passes wherever a text is asked for: a page's name, a component's name or
    /// description. Anything passed that is not one of these is a literal, and shows as written.
    std::string TranslationKey(const std::string& key) {
      return QualifyKey(TranslationPrefix(RunningScript()), key);
    }

    /// `translations::Prefix()` - the namespace this script's keys are under.
    std::string TranslationScriptPrefix() {
      return TranslationPrefix(RunningScript());
    }

    // -------------------------------------------------------------------------------------------------
    // Registration
    // -------------------------------------------------------------------------------------------------

    /// The parts every component kind shares: the type, the two reference-count behaviours and the
    /// conversion to the base. Every kind registers the same two behaviours - they work per object, not
    /// per type, and every kind's count is the base's at offset zero - and the same opImplCast, whose
    /// returned handle is the base one a script asked for.
    void RegisterComponentType(AngelScript::asIScriptEngine* const engine, const char* const name,
                               const char* const description) {
      util::RegisterObjectType(engine, name, 0, AngelScript::asOBJ_REF).Desc(description);

      // Spelled out rather than run through a helper, as the other reference types here are: a
      // behaviour's declaration is a fixed shape that has nothing to do with the thunk behind it.
      util::RegisterObjectBehaviour(engine, name, AngelScript::asBEHAVE_ADDREF, "void f()", AngelScript::asFUNCTION(ComponentAddRef),
                                    AngelScript::asCALL_CDECL_OBJFIRST);
      util::RegisterObjectBehaviour(engine, name, AngelScript::asBEHAVE_RELEASE, "void f()", AngelScript::asFUNCTION(ComponentRelease),
                                    AngelScript::asCALL_CDECL_OBJFIRST);
      util::RegisterObjectMethod(engine, name, "Component@ opImplCast() const", AngelScript::asFUNCTION(CastToComponent),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("The component as a Component, which is what every component kind is convertible to and what "
              "Submenu::Add takes. Implicit - a script never writes it.");
    }

    /// The AS spelling of a range's number.
    template <typename T>
    const char* RangeTypeName();

    template <>
    const char* RangeTypeName<std::int32_t>() {
      return "int";
    }

    template <>
    const char* RangeTypeName<float>() {
      return "float";
    }

    /// One of the four range types, which differ only in their number and in whether there is a checkbox
    /// on the row. The number is passed in as `value`, bounded by `min` and `max`, and moved in steps of
    /// `step`.
    template <typename T>
    void RegisterRangeType(AngelScript::asIScriptEngine* const engine, const char* const name, const bool togglable) {
      const char* const number = RangeTypeName<T>();

      RegisterComponentType(engine, name, "A number with a minimum, a maximum and a step.");

      const std::string factory = togglable
        ? std::format("{}@ f(const std::string &in name, const std::string &in description, {} value, {} min, {} max, {} step, bool toggled)",
                      name, number, number, number, number)
        : std::format("{}@ f(const std::string &in name, const std::string &in description, {} value, {} min, {} max, {} step)",
                      name, number, number, number, number);

      util::RegisterObjectBehaviour(engine, name, AngelScript::asBEHAVE_FACTORY, factory.c_str(),
                                    togglable ? AngelScript::asFunctionPtr(&CreateToggleRange<T>) : AngelScript::asFunctionPtr(&CreateRange<T>),
                                    AngelScript::asCALL_CDECL)
        .Desc(togglable
                ? "A number that can be changed with left and right, and switched on and off with the submit "
                  "key. Its state is the script's: it is read with Get and GetToggled and written with Set, "
                  "whether or not a page is being drawn."
                : "A number that can be changed with left and right. Its state is the script's: it is read "
                  "with Get and written with Set, whether or not a page is being drawn.")
        .Param("name", "What the row says, as a key or a literal: a key registered under the script's prefix "
                       "resolves, anything else shows as written.")
        .Param("description", "What the number is for. Empty asks for the name's own description, which is the "
                              "name with \"/desc\" after it and the namespaced spelling to match: a row named "
                              "translations::Key(\"level\") takes gui::translations::Register(\"level/desc\", ...).")
        .Param("value", "What the number starts at.")
        .Param("min", "The lowest it can be.")
        .Param("max", "The highest it can be.")
        .Param("step", "How much one press of left or right changes it by.")
        .Returns("The row.");

      util::RegisterObjectMethod(engine, name, std::format("{} Get() const", number).c_str(), AngelScript::asFunctionPtr(&RangeGet<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("What the number is now, wherever it was last set from.")
        .Returns("The value.");
      util::RegisterObjectMethod(engine, name, std::format("void Set({} value)", number).c_str(), AngelScript::asFunctionPtr(&RangeSet<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Sets the number, which the next frame of a page it is drawn on shows. The value is not clamped "
              "here: the menu clamps what it navigates to, and a script setting its own state is setting it "
              "to what it asked for.")
        .Param("value", "What to set it to.");
      util::RegisterObjectMethod(engine, name, std::format("{} GetMin() const", number).c_str(), AngelScript::asFunctionPtr(&RangeGetMin<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("The lowest the number can be, as it was made with.")
        .Returns("The minimum.");
      util::RegisterObjectMethod(engine, name, std::format("{} GetMax() const", number).c_str(), AngelScript::asFunctionPtr(&RangeGetMax<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("The highest the number can be, as it was made with.")
        .Returns("The maximum.");
      util::RegisterObjectMethod(engine, name, std::format("{} GetStep() const", number).c_str(), AngelScript::asFunctionPtr(&RangeGetStep<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("How much one press of left or right changes the number by, as it was made with.")
        .Returns("The step.");

      if (!togglable) {
        return;
      }

      util::RegisterObjectMethod(engine, name, "bool GetToggled() const", AngelScript::asFunctionPtr(&RangeGetToggled<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Whether the row is switched on, which is the checkbox beside the number and has nothing to do "
              "with where the number is.")
        .Returns("True when it is on.");
      util::RegisterObjectMethod(engine, name, "void SetToggled(bool toggled)", AngelScript::asFunctionPtr(&RangeSetToggled<T>),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Switches the row on or off, which the next frame of a page it is drawn on shows.")
        .Param("toggled", "Whether to switch it on.");
    }
  }

  bool InvokeUiCallback(const std::weak_ptr<Script>& owner, const std::string& declaration, ScriptSubmenu& sub) {
    if (declaration.empty()) {
      return false;
    }

    const std::shared_ptr<Script> script = owner.lock();
    if (!script) {
      return false;
    }

    auto* const fn = script->GetFunctionByDecl(declaration);
    if (!fn) {
      // The module this was declared in is still loaded, so the function it named is gone from it; the
      // page is left empty rather than the frame being drawn with nothing to draw.
      LOG_ERROR("[AS] The UI callback '{}' of a page cannot be found in the script it was declared in", declaration);
      return false;
    }

    // One reference, held by this call, and not the engine's to take or give: the callback is called with
    // the address of this variable, so what the engine counts is never touched. The variable is not const
    // for that reason - taking its address is how the callee is handed the handle.
    auto* handle = new SubmenuHandle(sub.shared_from_this());

    bool ran = false;
    try {
      ScriptContext context(fn->GetEngine());

      // Called, not started as a coroutine. The page is not built until this returns, so a UI callback
      // that yielded would be yielding to a frame already waiting for it - and one that parks at all
      // fails the call, which is reported and leaves the page empty for that frame.
      context.Run(fn, false, &handle);
      ran = true;
    } catch (const std::exception& e) {
      LOG_ERROR("[AS] The UI callback '{}' of a page threw: {}", declaration, e.what());
    }

    handle->Release();
    return ran;
  }

  void RegisterGui(AngelScript::asIScriptEngine* const engine) {
    engine->SetDefaultNamespace("gui");

    // ---------------------------------------------------------------------------------------------
    // The types everything else names
    // ---------------------------------------------------------------------------------------------

    // First, and in this order: a declaration is resolved as it is registered, so a funcdef or a method
    // naming a type that does not exist yet is refused outright, and everything below names both of
    // these.
    util::RegisterObjectType(engine, "Component", 0, AngelScript::asOBJ_REF)
      .Desc("What every component a script draws a page out of has in common: nothing but being one. A "
            "component is made by one of the gui functions below - gui::Toggle, gui::List, gui::Label - and "
            "handed to Submenu::Add, which is the only thing that takes one.");
    // The base's own behaviours, not just the derived types': a handle of the static type Component is
    // released by the engine through this type, so without them every component a script let go of would
    // leak - the same two functions serve both, since they work per object rather than per type.
    util::RegisterObjectBehaviour(engine, "Component", AngelScript::asBEHAVE_ADDREF, "void f()", AngelScript::asFUNCTION(ComponentAddRef),
                                  AngelScript::asCALL_CDECL_OBJFIRST);
    util::RegisterObjectBehaviour(engine, "Component", AngelScript::asBEHAVE_RELEASE, "void f()", AngelScript::asFUNCTION(ComponentRelease),
                                  AngelScript::asCALL_CDECL_OBJFIRST);

    util::RegisterObjectType(engine, "Submenu", 0, AngelScript::asOBJ_REF)
      .Desc("A page of the menu a script draws. It is made with the function of the same name, and drawn "
            "by the UI callback it was made with. A script's pages hang together: the one it registers "
            "with RegisterRoot is its own row on the root page of the menu, and the others are reached "
            "from it by a gui::Link.");
    util::RegisterObjectBehaviour(engine, "Submenu", AngelScript::asBEHAVE_ADDREF, "void f()", AngelScript::asFUNCTION(SubmenuAddRef),
                                  AngelScript::asCALL_CDECL_OBJFIRST);
    util::RegisterObjectBehaviour(engine, "Submenu", AngelScript::asBEHAVE_RELEASE, "void f()", AngelScript::asFUNCTION(SubmenuRelease),
                                  AngelScript::asCALL_CDECL_OBJFIRST);

    // ---------------------------------------------------------------------------------------------
    // The callbacks
    // ---------------------------------------------------------------------------------------------

    // Before the components, because their factories name them: `Execute` and `Link` take one of these as
    // their handler, and a funcdef is a type like any other to a declaration that names it.
    util::RegisterFuncdef(engine, "void ActionCallback()")
      .Desc("The shape of a callback that takes nothing and returns nothing, which is what an Execute row "
            "and a Link's on-open take: any global function of that shape can be handed over, as "
            "gui::Execute(\"go\", \"\", @OnGo).");

    util::RegisterFuncdef(engine, "void UiCallback(Submenu@ &in sub)")
      .Desc("The shape of the function a page is drawn by: one parameter, the page being drawn. Any global "
            "function of that shape can be made a page's, as gui::Submenu(\"menu\", @OnRootUi). It is "
            "called once per frame while the page is shown, on the render thread, and every component it "
            "adds is drawn in that frame. The parameter is a reference to a handle, so it is written "
            "`void OnRootUi(gui::Submenu@ &in sub)`. It must not wait, sleep or yield: the frame is waiting "
            "for it to return, and a callback that parks fails the call and leaves the page empty.")
      .Param("sub", "The page being drawn, to be added to.");

    // ---------------------------------------------------------------------------------------------
    // The components
    // ---------------------------------------------------------------------------------------------

    RegisterComponentType(engine, "Label", "A line of centred text, which cannot be selected and is not a row "
                                           "a player can land on.");
    util::RegisterObjectBehaviour(engine, "Label", AngelScript::asBEHAVE_FACTORY, "Label@ f(const std::string &in text)",
                                  AngelScript::asFUNCTION(CreateLabel), AngelScript::asCALL_CDECL)
      .Desc("A line of centred text, drawn as [text]. A label is content rather than a row: it says something "
            "between the rows around it.")
      .Param("text", "What it says, as a key or a literal.")
      .Returns("The label.");

    RegisterComponentType(engine, "Toggle", "A checkbox: something that is on or off, switched with the submit "
                                            "key.");
    util::RegisterObjectBehaviour(engine, "Toggle", AngelScript::asBEHAVE_FACTORY,
                                  "Toggle@ f(const std::string &in name, const std::string &in description, bool value)",
                                  AngelScript::asFUNCTION(CreateToggle), AngelScript::asCALL_CDECL)
      .Desc("A checkbox with a name and a description. Its state is the script's: it is read with Get and "
            "written with Set or by the player pressing submit on it, whether or not a page is being drawn.")
      .Param("name", "What the row says, as a key or a literal.")
      .Param("description", "What the checkbox is for. Empty asks for the name's own description, which is the "
                            "name with \"/desc\" after it and the namespaced spelling to match.")
      .Param("value", "Whether it starts on.")
      .Returns("The checkbox.");
    util::RegisterObjectMethod(engine, "Toggle", "bool Get() const", AngelScript::asFUNCTION(ToggleGet), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether the checkbox is on now, wherever it was last set from - a script's Set or the player "
            "pressing submit on it.")
      .Returns("True when it is on.");
    util::RegisterObjectMethod(engine, "Toggle", "void Set(bool value)", AngelScript::asFUNCTION(ToggleSet), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Switches the checkbox, which the next frame of a page it is drawn on shows.")
      .Param("value", "Whether to switch it on.");

    RegisterComponentType(engine, "List", "A list of items, moved through with left and right.");
    util::RegisterObjectBehaviour(engine, "List", AngelScript::asBEHAVE_FACTORY,
                                  "List@ f(const std::string &in name, const std::string &in description, const std::array<std::string> &in items, int index)",
                                  AngelScript::asFUNCTION(CreateList), AngelScript::asCALL_CDECL)
      .Desc("A list of items with a name and a description, starting at one of them. Its state is the "
            "script's: the items and the selection are read back with Get, GetItem and GetItemCount, and "
            "written with Set and SetItems, whether or not a page is being drawn.")
      .Param("name", "What the row says, as a key or a literal.")
      .Param("description", "What the list is for. Empty asks for the name's own description, which is the name "
                            "with \"/desc\" after it and the namespaced spelling to match.")
      .Param("items", "What the list holds, each item a key or a literal. The array is copied, so the list "
                      "keeps what it was given however the script changes its own.")
      .Param("index", "Which item is selected at the start. Clamped to the list.")
      .Returns("The list.");

    RegisterComponentType(engine, "ToggleList", "A list of items with a checkbox on the same row.");
    util::RegisterObjectBehaviour(engine, "ToggleList", AngelScript::asBEHAVE_FACTORY,
                                  "ToggleList@ f(const std::string &in name, const std::string &in description, const std::array<std::string> &in items, int index, bool toggled)",
                                  AngelScript::asFUNCTION(CreateToggleList), AngelScript::asCALL_CDECL)
      .Desc("A list with a checkbox: the items are moved through with left and right and the checkbox is "
            "switched with submit, and the two are independent of each other.")
      .Param("name", "What the row says, as a key or a literal.")
      .Param("description", "What the list is for. Empty asks for the name's own description, which is the name "
                            "with \"/desc\" after it and the namespaced spelling to match.")
      .Param("items", "What the list holds, each item a key or a literal.")
      .Param("index", "Which item is selected at the start. Clamped to the list.")
      .Param("toggled", "Whether the checkbox starts on.")
      .Returns("The list.");

    for (const char* const type : {"List", "ToggleList"}) {
      util::RegisterObjectMethod(engine, type, "int Get() const", AngelScript::asFUNCTION(ListGet), AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Which item is selected now, counted from zero, wherever it was last set from.")
        .Returns("The index.");
      util::RegisterObjectMethod(engine, type, "void Set(int index)", AngelScript::asFUNCTION(ListSet), AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Selects an item, which the next frame of a page it is drawn on shows. An index outside the "
              "list is clamped into it.")
        .Param("index", "Which item to select, counted from zero.");
      util::RegisterObjectMethod(engine, type, "int GetItemCount() const", AngelScript::asFUNCTION(ListGetItemCount),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("How many items the list holds now.")
        .Returns("The number of items.");
      util::RegisterObjectMethod(engine, type, "std::string GetItem(int index) const", AngelScript::asFUNCTION(ListGetItem),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("The item at an index, as it was given - the key rather than what it resolves to, so that a "
              "script can match on what it put in. Reported and empty for an index the list does not have.")
        .Param("index", "Which item to read, counted from zero.")
        .Returns("The item, or an empty string.");
      util::RegisterObjectMethod(engine, type, "void SetItems(const std::array<std::string> &in items)", AngelScript::asFUNCTION(ListSetItems),
                                 AngelScript::asCALL_CDECL_OBJFIRST)
        .Desc("Replaces every item, which the next frame of a page it is drawn on shows. The selection is "
              "clamped into the new list rather than reset, so a list rebuilt every tick keeps where the "
              "player had got to.")
        .Param("items", "What the list holds now, each item a key or a literal.");
    }

    util::RegisterObjectMethod(engine, "ToggleList", "bool GetToggled() const", AngelScript::asFUNCTION(ListGetToggled),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether the checkbox is on, which is independent of which item is selected.")
      .Returns("True when it is on.");
    util::RegisterObjectMethod(engine, "ToggleList", "void SetToggled(bool toggled)", AngelScript::asFUNCTION(ListSetToggled),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Switches the checkbox, which the next frame of a page it is drawn on shows.")
      .Param("toggled", "Whether to switch it on.");

    RegisterRangeType<std::int32_t>(engine, "IntRange", false);
    RegisterRangeType<std::int32_t>(engine, "ToggleIntRange", true);
    RegisterRangeType<float>(engine, "FloatRange", false);
    RegisterRangeType<float>(engine, "ToggleFloatRange", true);

    RegisterComponentType(engine, "Execute", "A row that runs something when it is pressed.");
    util::RegisterObjectBehaviour(engine, "Execute", AngelScript::asBEHAVE_FACTORY,
                                  "Execute@ f(const std::string &in name, const std::string &in description, const ActionCallback@+ handler)",
                                  AngelScript::asFUNCTION(CreateExecute), AngelScript::asCALL_CDECL)
      .Desc("A row that runs a function of the script when the player presses submit on it. The callback "
            "runs where the menu's own handlers run - on the thread pool - so it may do whatever a handler "
            "reached from the menu may do, including waiting.")
      .Param("name", "What the row says, as a key or a literal.")
      .Param("description", "What pressing it does. Empty asks for the name's own description, which is the "
                            "name with \"/desc\" after it and the namespaced spelling to match.")
      .Param("handler", "The function to run, passed as @OnApply. It takes nothing and returns nothing.")
      .Returns("The row.");

    RegisterComponentType(engine, "Link", "A row that goes to another page.");
    util::RegisterObjectBehaviour(engine, "Link", AngelScript::asBEHAVE_FACTORY,
                                  "Link@ f(const std::string &in name, const std::string &in description, const Submenu@+ target, const ActionCallback@+ on_open)",
                                  AngelScript::asFUNCTION(CreateLink), AngelScript::asCALL_CDECL)
      .Desc("A row that goes to another page when the player presses submit on it, the same way a row of the "
            "menu's own goes to one of its pages. The page does not have to be registered as a root to be "
            "reachable this way - that is what makes a sub-page a sub-page.")
      .Param("name", "What the row says, as a key or a literal.")
      .Param("description", "What the page is. Empty asks for the name's own description, which is the name "
                            "with \"/desc\" after it and the namespaced spelling to match.")
      .Param("target", "The page to go to, which must be one this script made.")
      .Param("on_open", "A function to run on the way, or null for none. It takes nothing and returns "
                        "nothing, and the navigation does not wait for it.")
      .Returns("The row.");

    // ---------------------------------------------------------------------------------------------
    // The page
    // ---------------------------------------------------------------------------------------------

    util::RegisterObjectBehaviour(engine, "Submenu", AngelScript::asBEHAVE_FACTORY,
                                  "Submenu@ f(const std::string &in name, const UiCallback@+ ui)",
                                  AngelScript::asFUNCTION(CreateSubmenuWithUi), AngelScript::asCALL_CDECL)
      .Desc("A new page, owned by the script that made it and drawn by the given callback every frame it is "
            "shown. A page a script only reaches from another of its own is made here and linked to; the "
            "one it registers with RegisterRoot is the page the menu lists under the script's name.")
      .Param("name", "The page's name, as a key or a literal: translations::Key(\"menu\") for a translated "
                     "one, or a literal to say it as written.")
      .Param("ui", "The function that draws the page, passed as @OnRootUi. It takes one parameter, the page "
                   "being drawn, declared `gui::Submenu@ &in sub`.")
      .Returns("The new page.");
    util::RegisterObjectBehaviour(engine, "Submenu", AngelScript::asBEHAVE_FACTORY, "Submenu@ f(const std::string &in name)",
                                  AngelScript::asFUNCTION(CreateSubmenuWithoutUi), AngelScript::asCALL_CDECL)
      .Desc("A new page drawn by nothing, which is only useful for one that is registered so that something "
            "else can list it: a page's own rows are drawn by the callback it was made with.")
      .Param("name", "The page's name, as a key or a literal.")
      .Returns("The new page.");

    util::RegisterObjectMethod(engine, "Submenu", "bool Add(const Component@+ comp)", AngelScript::asFUNCTION(SubmenuAdd),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Adds one component to the page being drawn, which is the page whose UI callback is running. "
            "A component is made and thrown away in the same statement - Add(gui::Toggle(...)) - and what "
            "it holds is kept for as long as the row is, so a component a script does not keep a handle to "
            "is still the component it meant.")
      .Param("comp", "The component to add: a Label, a Toggle, a List, a ToggleList, a range, an Execute or "
                     "a Link.")
      .Returns("True when it was added. False, and reported, for a component added outside of a page being "
               "drawn - which a script calling Add once its draw callback has returned is doing, there being "
               "no frame for it to be part of.");
    util::RegisterObjectMethod(engine, "Submenu", "bool AddOption(const Option@+ opt, const std::string &in label)",
                               AngelScript::asFUNCTION(SubmenuAddOption), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Draws an option's own row on the page being drawn: for one of the menu's own, whatever the menu "
            "draws for it, and for one a script registered, the UI callback that option was given. This is "
            "how a script's page holds an option that behaves exactly as it does everywhere else, with the "
            "same hotkey and the same saving.")
      .Param("opt", "The option to draw: one from Options::Find, or the script's own.")
      .Param("label", "Handed to the option for its row. A script's own option names its own components, so "
                      "it has no use for it; the menu's own options draw the label.")
      .Returns("True when it was drawn. False, and reported, outside of a page being drawn.");
    util::RegisterObjectMethod(engine, "Submenu", "uint64 GetId() const", AngelScript::asFUNCTION(SubmenuGetId),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Which page this is, as the menu numbers them. It is not stable across runs of the script: a "
            "reload gives the page a new one, so nothing a script stores may be keyed by it.")
      .Returns("The page's id.");
    util::RegisterObjectMethod(engine, "Submenu", "std::string GetName() const", AngelScript::asFUNCTION(SubmenuGetName),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The page's name, as it was made with - the key rather than what it resolves to.")
      .Returns("The name.");
    util::RegisterObjectMethod(engine, "Submenu", "bool RegisterRoot()", AngelScript::asFUNCTION(SubmenuRegisterRoot),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Puts a link to this page on the root page of the menu, below everything the menu put there "
            "itself, under the name of the script that made it. A script has one root - the row a player "
            "finds it by - so this is false, and reported, for a second page of the same script: the pages "
            "below the first are reached from it by a Link. Idempotent - registering the same page twice "
            "leaves one link - and false for a page that has been removed, since that would put a dead row "
            "in front of the player.")
      .Returns("True when the page is a root.");
    util::RegisterObjectMethod(engine, "Submenu", "bool Remove()", AngelScript::asFUNCTION(SubmenuRemove),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Takes this page away: the row on the root page goes, and a player standing on it is put back "
            "on the page underneath. A script that unloads has this done for it, so a script only calls it "
            "to drop a page while it is still loaded. False when there was nothing left to remove, which "
            "makes a second call harmless.")
      .Returns("True when the page was there.");
    util::RegisterObjectMethod(engine, "Submenu", "bool RemoveChild(const Submenu@+ page)", AngelScript::asFUNCTION(SubmenuRemoveChild),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Takes away another page of this script, which is how a script that makes pages as it goes drops "
            "one without dropping all of them. A page of another script is refused, and so is this one - "
            "which is what Remove is for.")
      .Param("page", "The page to take away, which this script must have made.")
      .Returns("True when it was there.");

    // ---------------------------------------------------------------------------------------------
    // The translations
    // ---------------------------------------------------------------------------------------------

    engine->SetDefaultNamespace("gui::translations");

    util::RegisterGlobalFunction(engine, "bool Register(const std::string &in key, const std::string &in value)",
                                 AngelScript::asFUNCTION(RegisterTranslation), AngelScript::asCALL_CDECL)
      .Desc("Registers one string for this script, which every text the script passes under that key then "
            "shows instead of the key. The string is never written to disk: it lives beside the menu's own "
            "translations rather than in them, and goes when the script does, so a script cannot change "
            "what the menu says or leave anything behind. False when the menu has a string of its own under "
            "the resulting key - keys are prefixed with the script's own, so that cannot be reached - or "
            "when another script already holds it.")
      .Param("key", "The short name the script reads it back by, without the prefix: \"spawn\" becomes the "
                    "key \"<prefix>/spawn\", which is what translations::Key returns.")
      .Param("value", "What the text says.")
      .Returns("True when it was registered.");
    util::RegisterGlobalFunction(engine, "bool Remove(const std::string &in key)",
                                 AngelScript::asFUNCTION(RemoveTranslation), AngelScript::asCALL_CDECL)
      .Desc("Takes away a string this script registered, after which the key shows as written again. False "
            "when this script has no string under it, so that a second Remove is harmless.")
      .Param("key", "The short name it was registered under.")
      .Returns("True when it was removed.");
    util::RegisterGlobalFunction(engine, "std::string Key(const std::string &in key)",
                                 AngelScript::asFUNCTION(TranslationKey), AngelScript::asCALL_CDECL)
      .Desc("The short name a script writes, as the key the menu resolves: what a script passes wherever a "
            "text is asked for - a page's name, a component's name or description - so that the text shows "
            "the string registered under it. A short name nothing is registered under shows as the full key "
            "rather than as the short one, so a script that wants a literal should pass it as it is.")
      .Param("key", "The short name, as it was registered.")
      .Returns("The full key.");
    util::RegisterGlobalFunction(engine, "std::string Prefix()", AngelScript::asFUNCTION(TranslationScriptPrefix),
                                 AngelScript::asCALL_CDECL)
      .Desc("The namespace this script's translation keys live in, which is the same prefix its options are "
            "registered under: its manifest's short_name, or its own name made into one.")
      .Returns("The prefix.");

    // Back to the global namespace: everything below is not in the GUI, and a registration that landed in
    // it by accident would be unreachable from a script.
    engine->SetDefaultNamespace("");

    // ---------------------------------------------------------------------------------------------
    // The option's own UI
    // ---------------------------------------------------------------------------------------------

    // Registered here rather than in the option binding because the funcdef they take only exists once
    // this has run - which is why RegisterGui is called after RegisterOption, whose type these add
    // methods to.
    util::RegisterObjectMethod(engine, "Option", "bool SetUiCallback(const gui::UiCallback@+ fn)", AngelScript::asFunctionPtr(&option::SetOptionUiCallback),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Names the function that draws this option's row, which is what makes an option a script "
            "registered show its own components on a page - the same hook a C++ option implements to draw "
            "its own. The function is called once per frame while a page drawing this option is shown, and "
            "the components it adds are this option's: they get its hotkey and are saved with it. False for "
            "an option of the menu's own, which already knows how to draw itself.")
      .Param("fn", "The function that draws the row, passed as @OnUi. It takes one parameter, the page being "
                   "drawn, declared `gui::Submenu@ &in sub`.")
      .Returns("True when it was set.");
    util::RegisterObjectMethod(engine, "Option", "bool HasUiCallback() const", AngelScript::asFunctionPtr(&option::HasOptionUiCallback),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this option has a UI callback to draw its row with - set with SetUiCallback, or, for an "
            "option of the menu's own, never, since the menu draws its own rows.")
      .Returns("True when there is one.");
  }
}
