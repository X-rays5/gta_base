//
// Created by X-ray on 12/09/2026.
//

#include "as_option.hpp"

#include "../script/as_script_context.hpp"
#include "../script/as_script_manager.hpp"
#include "../util/as_bind.hpp"
#include "../../options/option_registry.hpp"
#include "../../script/game_task_executor.hpp"
#include "../../ui/script_gui/script_submenu.hpp"

#include <angelscript/scriptarray/scriptarray.h>
#include <minicoropp/coroutine.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// A script's `Option` is the menu's own option registry seen from a script, and this file is the whole
// of the layer between the two: the registry and the command line know nothing about scripts, and the
// two classes above know nothing about the registry.
//
// Two things about that layer are worth knowing before reading it.
//
// The first is where a callback's arguments come from. A callback is handed a handle of its own - see
// ScriptOptionHandle - which holds the parse of that one run, so a callback that runs another option
// does not disturb the arguments it is reading, and nothing has to be unwound if one of them throws.
// The handle is made per run, here, and released by the run that made it.
//
// The second is what a script's option is called. A script does not own the flat namespace of option
// names - nine of them are the menu's, and every other script is registering its own into the same
// one - so an option a script makes is registered under the script's prefix and the name it was made
// with, as in `optreg.set_level`, and the prefix is the manifest's to shorten. A name a script reads
// is therefore read twice: as written, so that nothing is shadowed, and then as one of the calling
// script's own, so that a script can still name its own options the way it declared them.
//
// The third is why a callback is called with `Option@ &in self` rather than `Option@ self`. The engine
// reference-counts the arguments of a handle parameter it is given with SetArgObject, and releases that
// reference only when the call ends in an exception - a call that finishes leaves it outstanding, which
// is a reference leaked per run, and one released twice when the callback throws. A parameter declared
// as a reference is not the engine's to count at all: the address of a handle variable goes on the
// stack, the VM reads the handle from there, and neither the normal nor the exception path takes or
// gives a reference to it. The handle is therefore created with the one reference that keeps it alive
// for the call and released by the run afterwards, which is symmetric on every path and needs nothing
// known about how the engine cleans up. SetCallback is where that shape is insisted on, since a
// callback without it cannot be called this way.
namespace base::menu::as::bindings::option {
  namespace {
    using menu::options::BaseOption;
    using menu::options::CommandArg;
    using menu::options::FindArg;
    using menu::options::IsValidArgType;
    using menu::options::ParsedArg;
    using menu::options::ReadArgs;
    using script::Script;
    using script::ScriptContext;
    using script::ScriptManager;

    /// Every handle that exists, counted by the two ends of its life - see LiveInstances.
    std::atomic<std::int64_t> live_instances{0};

    /// The same name with its leading dashes taken off, so that "--level" and "level" are one name.
    std::string_view WithoutDashes(std::string_view name) {
      while (!name.empty() && name.front() == '-') {
        name.remove_prefix(1);
      }
      return name;
    }

    /// Whether `args` already declares `name`, under either spelling of it.
    ///
    /// The dashes are ignored on both sides so that one argument cannot be declared twice under two
    /// names, which the getters - that read an argument by either spelling - could not tell apart.
    bool DeclaresArg(const std::vector<CommandArg>& args, const std::string_view name) {
      for (const auto& arg : args) {
        if (arg.name == name || WithoutDashes(arg.name) == WithoutDashes(name)) {
          return true;
        }
      }

      return false;
    }

    /// `arg` as the integer it can be read as.
    ///
    /// A flag's value is in its own field, so it reads as the 1 or 0 it was set to rather than as the
    /// zero its integer field is left at - which is what makes asking a flag for a number answer the
    /// question that was asked.
    std::int64_t ValueAsInteger(const ParsedArg& arg) {
      if (arg.type == 'b') {
        return arg.boolean ? 1 : 0;
      }

      return arg.integer;
    }

    /// `arg` as the number it can be read as, on the same terms.
    double ValueAsNumber(const ParsedArg& arg) {
      if (arg.type == 'b') {
        return arg.boolean ? 1.0 : 0.0;
      }

      return arg.number;
    }

    /// `arg` as the boolean it can be read as: a flag is whether it was given, and anything else is
    /// whether it holds something other than zero.
    bool ValueAsBool(const ParsedArg& arg) {
      if (arg.type == 'b') {
        return arg.boolean;
      }

      return arg.number != 0.0 || arg.integer != 0;
    }

    /// The script running right now, by name and by handle, which are the same fact: the name is what
    /// the registry is told to unregister a script's options by when it unloads, and the handle is what
    /// resolves a callback again inside the script that declared it.
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

    /// The option `name` names, as `owner` and the registry together read it.
    ///
    /// The name is read as written first, so a script can never shadow an option that is registered
    /// under exactly that name: the menu's own options and another script's answer to their full,
    /// prefixed names however the running script is called. Only a name that names nothing there is
    /// read again as one of `owner`'s own, which is what lets a script say `Options::Run("set_level",
    /// ...)` about the option it registered as `optreg.set_level` instead of repeating its own prefix
    /// at every call - the second reading is the same name, qualified the way its registration was.
    std::shared_ptr<BaseOption> ResolveOption(const std::shared_ptr<Script>& owner, const std::string& name) {
      if (!menu::options::kOPTION_REGISTRY) {
        return nullptr;
      }

      if (const std::shared_ptr<BaseOption> option = menu::options::kOPTION_REGISTRY->GetOption(name)) {
        return option;
      }

      if (!owner) {
        // No script is running, so there is no second reading of the name to try - which is the case a
        // native outside the VM asks from.
        return nullptr;
      }

      const std::string qualified = owner->QualifyOptionName(name);
      if (qualified == name) {
        // There is no prefix to put in front of it, so this would be the same lookup over again.
        return nullptr;
      }

      return menu::options::kOPTION_REGISTRY->GetOption(qualified);
    }

    /// Whether `fn` is the shape a callback is called with: `void fn(Option@ &in self)`, and for an
    /// option's UI callback `void fn(gui::Submenu@ &in sub)`, which is the same shape over a handle of
    /// another type - which is all this can see, and all it needs to: the funcdef a callback is handed
    /// over as is what checks the type.
    ///
    /// The funcdef parameter of SetCallback is what really enforces this - a function of any other shape
    /// is not an `OptionCallback` and the compiler refuses to convert it - so this is the second lock on
    /// the same door, and the place the report names the shape a script got wrong.
    bool IsCallbackShape(AngelScript::asIScriptFunction* fn) {
      if (fn->GetReturnTypeId() != AngelScript::asTYPEID_VOID || fn->GetParamCount() != 1) {
        return false;
      }

      int type_id = 0;
      AngelScript::asDWORD flags = 0;
      if (fn->GetParam(0, &type_id, &flags, nullptr, nullptr) < 0) {
        return false;
      }

      // A handle, and one the callee is given as a reference: that pair is what makes the argument the
      // caller's to hold rather than the engine's to count, and the reason the address of a handle
      // variable is what the call is handed.
      return (type_id & AngelScript::asTYPEID_OBJHANDLE) != 0 &&
             (flags & AngelScript::asTM_INOUTREF) == AngelScript::asTM_INREF;
    }

    /// `Option("name", "description")` - a new option, owned by the script that made it and empty until
    /// it is given arguments and a callback.
    ScriptOptionHandle* Create(const std::string& name, const std::string& description) {
      // Nothing to qualify, and nothing a script could run: a name is what an option is found by, and
      // the separator in front of an empty one would be a name no script could write.
      if (name.empty()) {
        LOG_ERROR("[AS] An option was made without a name, so there would be nothing to register it under "
                  "and nothing to run it by");
        return nullptr;
      }

      std::string owner_name;
      const std::weak_ptr<Script> owner = RunningScript(owner_name);
      const std::shared_ptr<Script> owner_script = owner.lock();
      if (!owner_script) {
        LOG_ERROR("[AS] Option('{}') was made outside of a loaded script, so there is nothing to run its "
                  "callback with and nothing to unregister it from", name);
        return nullptr;
      }

      // Qualified here rather than at the registration, which is where a name is decided: the option is
      // one of the script's from the moment it is made, so the name it reports is the name it is
      // registered under, the name its help says, and the name it is run by.
      const std::string qualified = owner_script->QualifyOptionName(name);

      // The one reference this handle starts with is the factory's, which is the one the engine takes
      // for the script's side of the assignment.
      return new ScriptOptionHandle(std::make_shared<ScriptOption>(qualified, description, owner_name, owner), nullptr);
    }

    void AddRef(ScriptOptionHandle* self) {
      self->AddRef();
    }

    void Release(ScriptOptionHandle* self) {
      self->Release();
    }

    bool AddArg(ScriptOptionHandle* self, const std::string& name, const std::string& type, const std::string& help) {
      return self->AddArg(name, type, help);
    }

    bool SetCallback(ScriptOptionHandle* self, AngelScript::asIScriptFunction* fn) {
      return self->SetCallback(fn);
    }

    std::string GetName(const ScriptOptionHandle* self) {
      return self->GetName();
    }

    std::string GetDescription(const ScriptOptionHandle* self) {
      return self->GetDescription();
    }

    std::string GetHelp(const ScriptOptionHandle* self) {
      return self->GetHelp();
    }

    bool WasUsed(const ScriptOptionHandle* self, const std::string& name) {
      return self->WasUsed(name);
    }

    bool GetBool(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetBool(name);
    }

    std::int32_t GetInt(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetInt(name);
    }

    std::int64_t GetInt64(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetInt64(name);
    }

    float GetFloat(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetFloat(name);
    }

    double GetDouble(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetDouble(name);
    }

    std::string GetString(const ScriptOptionHandle* self, const std::string& name) {
      return self->GetString(name);
    }

    /// `Options::Register(opt)` - named apart from the registration function of this file, which is the
    /// one thing at namespace scope here that the name could be taken for.
    bool RegisterScriptOption(ScriptOptionHandle* self) {
      return self->Register();
    }

    /// `Options::Find(name)` - a handle naming the option registered under `name`, or null.
    ScriptOptionHandle* FindOption(const std::string& name) {
      if (!menu::options::kOPTION_REGISTRY) {
        LOG_ERROR("[AS] Options::Find('{}') was called outside of the menu, which has no options registered", name);
        return nullptr;
      }

      const std::shared_ptr<BaseOption> option = ResolveOption(RunningScript(), name);
      if (!option) {
        LOG_ERROR("[AS] Options::Find was given '{}', which is not the name of a registered option", name);
        return nullptr;
      }

      // No parse: this names an option rather than a run of one, and the getters a callback reads its
      // arguments with say so rather than answering a zero.
      return new ScriptOptionHandle(option, nullptr);
    }

    /// `Options::All()` - every registered option, each as a handle. A new array of its own, so a script
    /// that holds it holds handles and not the registry.
    AngelScript::CScriptArray* GetAllOptions() {
      if (!menu::options::kOPTION_REGISTRY) {
        LOG_ERROR("[AS] Options::All was called outside of the menu, which has no options registered");
        return nullptr;
      }

      AngelScript::asIScriptContext* const context = AngelScript::asGetActiveContext();
      if (!context) {
        LOG_ERROR("[AS] Options::All was called outside of a script, where there is no engine to build an "
                  "array with");
        return nullptr;
      }

      const std::vector<std::shared_ptr<BaseOption>> options = menu::options::kOPTION_REGISTRY->GetAllOptions();

      // The declaration is spelled as the add-on registers the type, which is `std::array<T>` in the
      // namespace the add-ons are registered in; a script writes `array<Option@>`, which is the default
      // array type and resolves to this one.
      AngelScript::asITypeInfo* const array_type = context->GetEngine()->GetTypeInfoByDecl("std::array<Option@>");
      if (!array_type) {
        LOG_ERROR("[AS] Options::All could not find the array type to hand the options back in, which is a "
                  "bug in the binding rather than something a script can cause");
        return nullptr;
      }

      auto* const array = AngelScript::CScriptArray::Create(array_type);
      if (!array) {
        LOG_ERROR("[AS] Options::All could not make an array to hand the options back in");
        return nullptr;
      }

      array->Resize(static_cast<AngelScript::asUINT>(options.size()));
      for (AngelScript::asUINT i = 0; i < static_cast<AngelScript::asUINT>(options.size()); ++i) {
        auto* const handle = new ScriptOptionHandle(options[i], nullptr);

        // SetValue takes the element's address and takes a reference of its own, so ours is dropped
        // again and the array is left holding the only one.
        void* element = handle;
        array->SetValue(i, &element);
        handle->Release();
      }

      return array;
    }

    /// `Options::Run(name, arguments)` - the option registered under `name`, run with `arguments` parsed
    /// the way its declared arguments say. False when there is no such option, when the line does not
    /// parse, and when the option's own run refused to start; a line that does not parse runs nothing.
    bool RunOption(const std::string& name, const std::string& arguments) {
      if (!menu::options::kOPTION_REGISTRY) {
        LOG_ERROR("[AS] Options::Run('{}') was called outside of the menu, which has no options to run", name);
        return false;
      }

      const std::shared_ptr<BaseOption> option = ResolveOption(RunningScript(), name);
      if (!option) {
        LOG_ERROR("[AS] Options::Run was given '{}', which is not the name of a registered option", name);
        return false;
      }

      return menu::options::ParseAndRun(option->GetCommand(), arguments);
    }
  }

  ScriptOption::ScriptOption(std::string name, std::string description, std::string owner_script, std::weak_ptr<script::Script> owner)
    : BaseOption(std::move(name), std::move(description)), owner_script_(std::move(owner_script)), owner_(std::move(owner)) {
  }

  bool ScriptOption::AddArg(const std::string& name, const std::string& type, const std::string& help) {
    if (name.empty()) {
      LOG_ERROR("[AS] An argument of '{}' was declared without a name, so there would be no way to give it a "
                "value or to read it back", GetName());
      return false;
    }

    // One character, because that is what the command line reads a value by - and checked here rather
    // than at the parse, so that a typo is reported by the script that declared it.
    if (type.size() != 1 || !IsValidArgType(type.front())) {
      LOG_ERROR("[AS] The argument '{}' of '{}' was declared as '{}', which is not a type: an argument is "
                "declared as 'i', 'u', 'f', 'd', 's' or 'b'", name, GetName(), type);
      return false;
    }

    if (DeclaresArg(args_, name)) {
      LOG_ERROR("[AS] '{}' already declares an argument named '{}'", GetName(), name);
      return false;
    }

    args_.push_back(CommandArg{name, type.front(), help});
    return true;
  }

  bool ScriptOption::SetCallback(AngelScript::asIScriptFunction* fn) {
    if (!fn) {
      LOG_ERROR("[AS] '{}' was given no callback, which leaves it with nothing to run", GetName());
      return false;
    }

    if (!IsCallbackShape(fn)) {
      LOG_ERROR("[AS] '{}' was given '{}' as its callback, which is not the shape a callback is called with: "
                "it takes one parameter, `Option@ &in self`", GetName(), fn->GetDeclaration());
      return false;
    }

    // The declaration rather than the function: the engine lends a callback to the native it is passed to
    // for the length of that call and no longer, and an option outlives every call that registered it.
    // Namespaces included, so that a function of one is found again from its declaration alone.
    callback_decl_ = fn->GetDeclaration(true, true);
    return true;
  }

  void ScriptOption::runCommand(std::shared_ptr<argparse::ArgumentParser> args) {
    if (!HasCallback()) {
      LOG_ERROR("[AS] '{}' was run, which has no callback to run: nothing happens", GetName());
      return;
    }

    // Read here rather than in the callback: the parser belongs to whoever ran the option, and what the
    // callback is given is the parse rather than the parser it came from.
    std::shared_ptr<const std::vector<ParsedArg>> parsed;
    if (args) {
      parsed = std::make_shared<const std::vector<ParsedArg>>(ReadArgs(*this, *args));
    }

    const std::string declaration = callback_decl_;
    const std::weak_ptr<script::Script> owner = owner_;

    // The option itself, held for as long as the run takes: the callback may outlive the handle the
    // script registered it with, and an option nothing holds would be one a running callback had lost.
    const std::shared_ptr<ScriptOption> self = weak_from_this().lock();
    if (!self) {
      LOG_ERROR("[AS] '{}' was run without the registry holding it, which cannot happen: the run is dropped", GetName());
      return;
    }

    auto* const executor = menu::script::kGAME_TASK_EXECUTOR;

    // A run is handed to the game thread rather than carried out where it was asked for, because that is
    // the thread the script's engine is driven from - and it is handed over even when it was asked for
    // from that very thread, since a callback reached from inside a script has to be driven by a context
    // of its own rather than by the one its caller is running in. Nothing here has to work out which
    // thread it is on: a queued task is picked up by the executor's next tick, whenever that is.
    if (executor) {
      executor->QueueTask([declaration, owner, self, parsed] {
        const std::shared_ptr<Script> owner_script = owner.lock();
        if (!owner_script) {
          // Unloaded between the run and the task's first turn. Nothing of the script is left to run.
          return;
        }

        auto* const fn = owner_script->GetFunctionByDecl(declaration);
        if (!fn) {
          // The module this was declared in is still loaded, so the function it named is gone from it;
          // there is nothing to run and nothing to report beyond that.
          LOG_ERROR("[AS] The callback '{}' of a run option cannot be found in the script it was declared in", declaration);
          return;
        }

        // One reference, held by this frame, and not the engine's to take or give: the callback is
        // called with the address of this variable, so what the engine counts is never touched. The
        // variable is not const for that reason - taking its address is how the callee is handed the
        // handle, and a pointer to a const pointer is not an address the call can be given.
        auto* self_handle = new ScriptOptionHandle(self, parsed);
        {
          ScriptContext context(fn->GetEngine());

          // A coroutine, so that a callback that yields or waits is waited for by the executor rather
          // than by this call - the same as it would be in GameTick.
          context.Run(fn, true, &self_handle);
          context.Tick();
          while (context.IsRunning() && !owner_script->IsUnloadRequested()) {
            minicoropp::this_coro::yield();
            context.Tick();
          }
        }
        self_handle->Release();
      });
      return;
    }

    // No executor to hand it to, which is a menu that never came up or a test driving its own: the run is
    // carried out here, to the callback's end. Nothing is driving a coroutine, so a callback that waits
    // has nothing to wait on and says so rather than parking anything.
    const std::shared_ptr<Script> owner_script = owner.lock();
    if (!owner_script) {
      LOG_ERROR("[AS] '{}' was run after the script that declared it was unloaded: nothing happens", GetName());
      return;
    }

    auto* const fn = owner_script->GetFunctionByDecl(declaration);
    if (!fn) {
      LOG_ERROR("[AS] The callback '{}' of '{}' cannot be found in the script it was declared in", declaration, GetName());
      return;
    }

    // Not const, for the same reason as in the queued run above: its address is the argument.
    auto* self_handle = new ScriptOptionHandle(self, parsed);
    {
      ScriptContext context(fn->GetEngine());
      context.Run(fn, false, &self_handle);
    }
    self_handle->Release();
  }

  bool ScriptOption::SetUiCallback(AngelScript::asIScriptFunction* fn) {
    if (!fn) {
      LOG_ERROR("[AS] '{}' was given no UI callback, which leaves its row with nothing to draw", GetName());
      return false;
    }

    if (!IsCallbackShape(fn)) {
      LOG_ERROR("[AS] '{}' was given '{}' as its UI callback, which is not the shape a UI callback is called "
                "with: it takes one parameter, `gui::Submenu@ &in sub`", GetName(), fn->GetDeclaration());
      return false;
    }

    // The declaration rather than the function, for the same reason the callback is held that way: a
    // page outlives every call, and the engine lends a function to a call and no longer.
    ui_decl_ = fn->GetDeclaration(true, true);
    return true;
  }

  void ScriptOption::CreateOptionUi(const std::string&, ui::Submenu*) {
    if (!HasUiCallback()) {
      // Contributes nothing, which is what an option with no UI of its own needs: the contract is "add
      // whatever components this option needs", and an option that needs none has met it.
      return;
    }

    // The page being built rather than the frame this was handed: the two are the same page - it is
    // BuildInto that draws this - and the page is what the UI callback has to be given a handle to,
    // since a component a script adds from inside the callback is added through it.
    ui::script_gui::ScriptSubmenu* const page = ui::script_gui::ScriptSubmenu::CurrentBuild();
    if (!page) {
      LOG_ERROR("[AS] '{}' has a UI callback to draw its row with, and was drawn where no page is being "
                "built: an option's row is drawn by a page, and there is no frame for it otherwise", GetName());
      return;
    }

    // The option is the page's current one for exactly as long as its own UI runs, which is what makes
    // every component the callback adds this option's - its hotkey, and saved with it.
    const ui::script_gui::ScriptSubmenu::OptionScope scope(this);
    static_cast<void>(gui::InvokeUiCallback(owner_, ui_decl_, *page));
  }

  ScriptOptionHandle::ScriptOptionHandle(std::shared_ptr<BaseOption> option,
                                         std::shared_ptr<const std::vector<ParsedArg>> args)
    : option_(std::move(option)), args_(std::move(args)) {
    live_instances.fetch_add(1, std::memory_order_relaxed);
  }

  void ScriptOptionHandle::AddRef() {
    refs_.fetch_add(1, std::memory_order_relaxed);
  }

  void ScriptOptionHandle::Release() {
    if (refs_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      live_instances.fetch_sub(1, std::memory_order_relaxed);
      delete this;
    }
  }

  std::string ScriptOptionHandle::GetName() const {
    return option_->GetName();
  }

  std::string ScriptOptionHandle::GetDescription() const {
    return option_->GetDescription();
  }

  std::string ScriptOptionHandle::GetHelp() const {
    return option_->GetHelp();
  }

  std::shared_ptr<ScriptOption> ScriptOptionHandle::AsScriptOption() const {
    return std::dynamic_pointer_cast<ScriptOption>(option_);
  }

  bool ScriptOptionHandle::AddArg(const std::string& name, const std::string& type, const std::string& help) {
    const std::shared_ptr<ScriptOption> script_option = AsScriptOption();
    if (!script_option) {
      LOG_ERROR("[AS] An argument was added to '{}', which the menu itself registered: only an option a script "
                "made declares its own arguments", GetName());
      return false;
    }

    return script_option->AddArg(name, type, help);
  }

  bool ScriptOptionHandle::SetCallback(AngelScript::asIScriptFunction* fn) {
    const std::shared_ptr<ScriptOption> script_option = AsScriptOption();
    if (!script_option) {
      LOG_ERROR("[AS] A callback was set on '{}', which the menu itself registered: an option of the menu's own "
                "already knows what to run", GetName());
      return false;
    }

    return script_option->SetCallback(fn);
  }

  bool ScriptOptionHandle::SetUiCallback(AngelScript::asIScriptFunction* fn) {
    const std::shared_ptr<ScriptOption> script_option = AsScriptOption();
    if (!script_option) {
      LOG_ERROR("[AS] A UI callback was set on '{}', which the menu itself registered: an option of the menu's "
                "own already has its row drawn for it", GetName());
      return false;
    }

    return script_option->SetUiCallback(fn);
  }

  bool ScriptOptionHandle::HasUiCallback() const {
    const std::shared_ptr<ScriptOption> script_option = AsScriptOption();
    return script_option && script_option->HasUiCallback();
  }

  bool ScriptOptionHandle::Register() {
    if (!menu::options::kOPTION_REGISTRY) {
      LOG_ERROR("[AS] '{}' was registered outside of the menu, which has no registry to register into", GetName());
      return false;
    }

    const std::shared_ptr<ScriptOption> script_option = AsScriptOption();
    if (!script_option) {
      LOG_ERROR("[AS] '{}' was registered, and the menu had already registered it itself: the menu's own options "
                "are in the registry and stay there", GetName());
      return false;
    }

    if (!script_option->HasCallback()) {
      LOG_ERROR("[AS] '{}' was registered before it was given a callback, so running it would do nothing: set one "
                "with SetCallback first", GetName());
      return false;
    }

    // The option is what the registry holds, not the handle: a script that lets go of the handle it made the
    // option with leaves the option registered, which is what registering it means.
    return menu::options::kOPTION_REGISTRY->AddOption(script_option);
  }

  const ParsedArg* ScriptOptionHandle::Find(const std::string& name) const {
    if (!args_) {
      LOG_ERROR("[AS] '{}' was asked for its arguments where there was no run of it to read them from - an option "
                "from Options::Find or Options::All names an option rather than a run, and GetHelp lists what it "
                "takes", GetName());
      return nullptr;
    }

    const ParsedArg* const found = FindArg(*args_, name);
    if (!found) {
      LOG_ERROR("[AS] '{}' was asked for the argument '{}', which it does not declare: GetHelp lists the "
                "arguments it takes", GetName(), name);
      return nullptr;
    }

    return found;
  }

  bool ScriptOptionHandle::WasUsed(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? arg->used : false;
  }

  bool ScriptOptionHandle::GetBool(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? ValueAsBool(*arg) : false;
  }

  std::int32_t ScriptOptionHandle::GetInt(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? static_cast<std::int32_t>(ValueAsInteger(*arg)) : 0;
  }

  std::int64_t ScriptOptionHandle::GetInt64(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? ValueAsInteger(*arg) : 0;
  }

  float ScriptOptionHandle::GetFloat(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? static_cast<float>(ValueAsNumber(*arg)) : 0.0F;
  }

  double ScriptOptionHandle::GetDouble(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? ValueAsNumber(*arg) : 0.0;
  }

  std::string ScriptOptionHandle::GetString(const std::string& name) const {
    const ParsedArg* const arg = Find(name);
    return arg ? arg->text : std::string{};
  }

  std::int64_t ScriptOptionHandle::LiveInstances() {
    return live_instances.load(std::memory_order_relaxed);
  }

  bool SetOptionUiCallback(ScriptOptionHandle* const self, AngelScript::asIScriptFunction* const fn) {
    return self->SetUiCallback(fn);
  }

  bool HasOptionUiCallback(const ScriptOptionHandle* const self) {
    return self->HasUiCallback();
  }

  void RegisterOption(AngelScript::asIScriptEngine* engine) {
    util::RegisterObjectType(engine, "Option", 0, AngelScript::asOBJ_REF)
      .Desc("An option: something that can be found by name and run with a command line. A script makes one, "
            "declares the arguments it takes, gives it a callback to run, and registers it with the menu - "
            "after which it is an option like any of the menu's own.");

    // The shape a callback is called with, which is what SetCallback takes and what the engine checks a
    // function against: a funcdef parameter is a type of its own, so nothing of another shape can be
    // handed over at all.
    //
    // After the type it names rather than before it: a declaration is resolved as it is registered, so
    // this one is refused outright while `Option` is still the name of nothing.
    util::RegisterFuncdef(engine, "void OptionCallback(Option@ &in self)")
      .Desc("The shape of the callback an option runs: one parameter, the option being run. Any global "
            "function of that shape can be handed to Option::SetCallback, as opt.SetCallback(@onLevel).");

    // Spelled out rather than run through the helpers, as the other reference types here are: a behaviour's
    // declaration is a fixed shape that has nothing to do with the thunk behind it.
    util::RegisterObjectBehaviour(engine, "Option", AngelScript::asBEHAVE_FACTORY, "Option@ f(const std::string &in, const std::string &in)",
                                  AngelScript::asFUNCTION(Create), AngelScript::asCALL_CDECL)
      .Desc("A new option, owned by the script that made it. Nothing about it is registered until "
            "Options::Register is called, so an option can be built up and thrown away. Its name is the "
            "script's own prefix and this name, as in optreg.set_level - the prefix is the manifest's "
            "short_name, or the script's own name, so that two scripts can each have an option called "
            "set_level.")
      .Param("name", "The name it is registered under and run by, without the script's prefix: "
                     "\"set_level\" becomes \"optreg.set_level\" in a script whose prefix is optreg.")
      .Param("description", "What the option is for, shown beside it in the menu and in its help.")
      .Returns("The new option.");
    util::RegisterObjectBehaviour(engine, "Option", AngelScript::asBEHAVE_ADDREF, "void f()", AngelScript::asFUNCTION(AddRef),
                                  AngelScript::asCALL_CDECL_OBJFIRST);
    util::RegisterObjectBehaviour(engine, "Option", AngelScript::asBEHAVE_RELEASE, "void f()", AngelScript::asFUNCTION(Release),
                                  AngelScript::asCALL_CDECL_OBJFIRST);

    util::RegisterObjectMethod(engine, "Option", "bool AddArg(const std::string &in, const std::string &in, const std::string &in)",
                               AngelScript::asFUNCTION(AddArg), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Declares one more argument the option takes, which is what gives it a command line and a line in "
            "its help text. False when the name is empty, the type is not one of the type characters, or an "
            "argument of that name is already declared.")
      .Param("name", "The argument as it is written on the command line, dashes included: \"--level\".")
      .Param("type", "How its value is read: \"i\" for an integer, \"u\" for an unsigned one, \"f\" for a float, "
                     "\"d\" for a double, \"s\" for a string, \"b\" for a flag that takes no value.")
      .Param("help", "What the argument is for, shown beside it in GetHelp.")
      .Returns("True when it was declared.");
    util::RegisterObjectMethod(engine, "Option", "bool SetCallback(const OptionCallback@+ fn)", AngelScript::asFUNCTION(SetCallback),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Names the function to run when the option is run, which is what makes it an option worth "
            "registering: a callback is given the option itself, and reads the arguments of that run from it.")
      .Param("fn", "The function to run, passed as opt.SetCallback(@onLevel).")
      .Returns("True when it was set.");

    util::RegisterObjectMethod(engine, "Option", "std::string GetName() const", AngelScript::asFUNCTION(GetName),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The name this option is found and run by, which for an option a script made is its own name "
            "with the script's prefix in front of it: the same name Options::All lists it under.")
      .Returns("The name.");
    util::RegisterObjectMethod(engine, "Option", "std::string GetDescription() const", AngelScript::asFUNCTION(GetDescription),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("What this option is for, as it was made with.")
      .Returns("The description.");
    util::RegisterObjectMethod(engine, "Option", "std::string GetHelp() const", AngelScript::asFUNCTION(GetHelp),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("What this option is and what it takes, as text: its name, its description, and one line per "
            "argument naming the type it reads and what it is for - which is how a script finds out what it "
            "can run an option with. A menu option's GetHelp answers as its own does.")
      .Returns("The help text.");

    util::RegisterObjectMethod(engine, "Option", "bool WasUsed(const std::string &in) const", AngelScript::asFUNCTION(WasUsed),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether the run this callback is reading gave the argument a value, which is how an argument that "
            "was left out is told from one that was given a zero. Reported and false when the argument is not "
            "one the option declares, or when there is no run to read.")
      .Param("name", "The argument to ask about, as it was declared or without its dashes.")
      .Returns("True when the command line carried it.");
    util::RegisterObjectMethod(engine, "Option", "bool GetBool(const std::string &in) const", AngelScript::asFUNCTION(GetBool),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as a boolean: a flag is whether it was given, anything else is whether it holds "
            "something other than zero.")
      .Param("name", "The argument to read.")
      .Returns("The value, or false when there is none to read - which is reported.");
    util::RegisterObjectMethod(engine, "Option", "int GetInt(const std::string &in) const", AngelScript::asFUNCTION(GetInt),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as an integer, truncated towards zero. A flag reads as its 1 or 0.")
      .Param("name", "The argument to read.")
      .Returns("The value, or zero when there is none to read - which is reported.");
    util::RegisterObjectMethod(engine, "Option", "int64 GetInt64(const std::string &in) const", AngelScript::asFUNCTION(GetInt64),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as a 64-bit integer, truncated towards zero.")
      .Param("name", "The argument to read.")
      .Returns("The value, or zero when there is none to read - which is reported.");
    util::RegisterObjectMethod(engine, "Option", "float GetFloat(const std::string &in) const", AngelScript::asFUNCTION(GetFloat),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as a float")
      .Param("name", "The argument to read.")
      .Returns("The value, or zero when there is none to read - which is reported.");
    util::RegisterObjectMethod(engine, "Option", "double GetDouble(const std::string &in) const", AngelScript::asFUNCTION(GetDouble),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as a double.")
      .Param("name", "The argument to read.")
      .Returns("The value, or zero when there is none to read - which is reported.");
    util::RegisterObjectMethod(engine, "Option", "std::string GetString(const std::string &in) const", AngelScript::asFUNCTION(GetString),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The argument as the text it was typed as, whichever type it was declared with.")
      .Param("name", "The argument to read.")
      .Returns("The value, or an empty string when there is none to read - which is reported.");

    engine->SetDefaultNamespace("Options");

    util::RegisterGlobalFunction(engine, "bool Register(Option@+ opt)", AngelScript::asFUNCTION(RegisterScriptOption),
                                 AngelScript::asCALL_CDECL)
      .Desc("Puts an option into the menu under its own name, which for an option a script made is that "
            "name with the script's prefix in front of it, after which it can be found by name and run. "
            "False when that name is already taken - the option that is there stays, and the menu's own "
            "options cannot be registered over - and when the option has no callback to run.")
      .Param("opt", "The option to register.")
      .Returns("True when it was registered.");
    util::RegisterGlobalFunction(engine, "Option@ Find(const std::string &in name)", AngelScript::asFUNCTION(FindOption),
                                 AngelScript::asCALL_CDECL)
      .Desc("The option registered under that name, or null when there is none. A name is read as written "
            "first, so the menu's own options and another script's are found by their full names; a name "
            "nothing is registered under is read again as one of the calling script's own, which is how a "
            "script finds `optreg.set_level` by writing Find(\"set_level\"). This is also how a script "
            "asks an option about itself: GetHelp says what it takes. The handle names an option rather "
            "than a run, so the getters a callback reads its arguments with answer with nothing here.")
      .Param("name", "The name the option is registered under, or the name of one of the calling script's "
                     "own options without its prefix.")
      .Returns("The option, or null.");
    util::RegisterGlobalFunction(engine, "std::array<Option@>@ All()", AngelScript::asFUNCTION(GetAllOptions),
                                 AngelScript::asCALL_CDECL)
      .Desc("Every option registered with the menu, the menu's own included, as a new array - which is how a "
            "script lists what it could run, each option's GetHelp saying what it takes.")
      .Returns("A new array of every registered option.");
    util::RegisterGlobalFunction(engine, "bool Run(const std::string &in name, const std::string &in arguments)",
                                 AngelScript::asFUNCTION(RunOption), AngelScript::asCALL_CDECL)
      .Desc("Runs the option registered under that name with `arguments` read the way the option's own "
            "arguments say - the same as if the line had been typed into the menu. The name is read the "
            "way Find reads one: as written first, then as one of the calling script's own. The line is "
            "parsed before anything runs, so an option is never run with half its arguments. False when "
            "there is no such option and when the line does not parse.")
      .Param("name", "The name the option is registered under, or the name of one of the calling script's "
                     "own options without its prefix.")
      .Param("arguments", "The command line, as it would be typed: \"--level 3 --flag\".")
      .Returns("True when the option was run.");

    // Back to the global namespace: a later registration must not land in `Options` by accident.
    engine->SetDefaultNamespace("");
  }
}
