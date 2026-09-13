//
// Created by X-ray on 11/09/2026.
//

#include "as_script.hpp"
#include "as_script_manager.hpp"
#include <angelscript.h>
#include <asjit.hpp>
#include "../../natives/natives_as.hpp"
#include "../../options/option_registry.hpp"
#include "../../script/general_task_executor.hpp"
#include "../../script/script_manager.hpp"
#include "../../ui/localization/manager.hpp"
#include "../../ui/script_gui/script_submenu_registry.hpp"
#include "../bindings/as_coro.hpp"
#include "../bindings/as_game_task.hpp"
#include "../bindings/as_gui.hpp"
#include "../bindings/as_lifecycle.hpp"
#include "../bindings/as_log.hpp"
#include "../bindings/as_mutex.hpp"
#include "../bindings/as_native_types.hpp"
#include "../bindings/as_notify.hpp"
#include "../bindings/as_option.hpp"
#include "../bindings/as_value.hpp"
#include "../util/as_bind.hpp"
#include "../util/as_generate_predefined.hpp"
#include "../util/as_script_logger.hpp"
#include "../util/as_util.hpp"
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <base-common/fs/vfs.hpp>
#include <minicoropp/coroutine.hpp>

namespace base::menu::as::script {
  namespace {
    void MessageCallback(const AngelScript::asSMessageInfo* msg) {
      switch (msg->type) {
        case AngelScript::asMSGTYPE_ERROR:
          LOG_ERROR("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        case AngelScript::asMSGTYPE_WARNING:
          LOG_WARN("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        case AngelScript::asMSGTYPE_INFORMATION:
          LOG_INFO("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        default:
          LOG_INFO("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
      }
    }

    int IncludeCallback(const char *include, const char *from, AngelScript::CScriptBuilder *builder, void *userParam) {
      const auto script_dir = static_cast<const std::filesystem::path*>(userParam);
      if (!script_dir) {
        return -1;
      }

      std::filesystem::path include_path = include;
      if (include_path.is_relative()) {
        // A relative include names a file beside the one that asked for it, which is how an include
        // reads anywhere else: a main file under src/ finds its neighbours there without spelling the
        // folder out, and a nested one goes on from wherever it is. The script's own directory is the
        // fallback, so a file at the root of a script stays reachable from one that is not.
        const std::filesystem::path from_dir = from ? std::filesystem::path(from).parent_path() : std::filesystem::path{};
        const std::filesystem::path beside = from_dir / include_path;
        include_path = std::filesystem::is_regular_file(beside) ? beside : *script_dir / include_path;
      }

      // Whatever was written - and whatever it resolved to - an include that ends up outside the
      // script's own directory is refused, and it is refused before anything is opened.
      if (!common::fs::vfs::EnsureIsWithinDirectory(*script_dir, include_path)) {
        LOG_ERROR("[AS] Include path '{}' requested by '{}' is outside of the script directory '{}'", include, from, script_dir->string());
        return -1;
      }

      return builder->AddSectionFromFile(include_path.string().c_str());
    }

    void SetLanguageModifications(AngelScript::asIScriptEngine* engine) {
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_EMPTY_LIST_ELEMENTS, true);
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE, true);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_UNSAFE_REFERENCES, false);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_MULTILINE_STRINGS, true);
      engine->SetEngineProperty(AngelScript::asEP_HEREDOC_TRIM_MODE, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_IMPLICIT_HANDLE_TYPES, false);
      engine->SetEngineProperty(AngelScript::asEP_REQUIRE_ENUM_SCOPE, true);
      engine->SetEngineProperty(AngelScript::asEP_PROPERTY_ACCESSOR_MODE, 3);
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_GLOBAL_VARS, false);
      engine->SetEngineProperty(AngelScript::asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALWAYS_IMPL_DEFAULT_COPY, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALTER_SYNTAX_NAMED_ARGS, 1);
      engine->SetEngineProperty(AngelScript::asEP_DISABLE_INTEGER_DIVISION, true);
      engine->SetEngineProperty(AngelScript::asEP_PRIVATE_PROP_AS_PROTECTED, false);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_UNICODE_IDENTIFIERS, true);
      engine->SetEngineProperty(AngelScript::asEP_IGNORE_DUPLICATE_SHARED_INTF, false);
      engine->SetEngineProperty(AngelScript::asEP_BOOL_CONVERSION_MODE, false);
      engine->SetEngineProperty(AngelScript::asEP_FOREACH_SUPPORT, true);
      engine->SetEngineProperty(AngelScript::asEP_MEMBER_INIT_MODE, 1);
    }

    void SetEngineBehaviors(AngelScript::asIScriptEngine* engine) {
      engine->SetEngineProperty(AngelScript::asEP_MAX_NESTED_CALLS, 100);
      engine->SetEngineProperty(AngelScript::asEP_OPTIMIZE_BYTECODE, true);
      engine->SetEngineProperty(AngelScript::asEP_COPY_SCRIPT_SECTIONS, true);
      engine->SetEngineProperty(AngelScript::asEP_INIT_STACK_SIZE, 1024 * 128);
      engine->SetEngineProperty(AngelScript::asEP_INIT_CALL_STACK_SIZE, 1024 * 32);
      engine->SetEngineProperty(AngelScript::asEP_BUILD_WITHOUT_LINE_CUES, false);
      engine->SetEngineProperty(AngelScript::asEP_INIT_GLOBAL_VARS_AFTER_BUILD, true);
      // Both have to be set before the first module is built: the optimizer drops every
      // asBC_JitEntry when the first is false (as_bytecode.cpp), and the compiler hook reads the
      // second to decide which asIJITCompiler interface to call. SetEngineProperties runs before
      // any module exists, which is what makes this the right place for them.
      engine->SetEngineProperty(AngelScript::asEP_INCLUDE_JIT_INSTRUCTIONS, true);
      engine->SetEngineProperty(AngelScript::asEP_JIT_INTERFACE_VERSION, 2);
      engine->SetEngineProperty(AngelScript::asEP_EXPAND_DEF_ARRAY_TO_TMPL, true);
      engine->SetEngineProperty(AngelScript::asEP_AUTO_GARBAGE_COLLECT, true);
      engine->SetEngineProperty(AngelScript::asEP_NO_DEBUG_OUTPUT, true);
    }

    void SetEngineProperties(AngelScript::asIScriptEngine* engine) {
      SetLanguageModifications(engine);
      SetEngineBehaviors(engine);
    }

    void RegisterBindings(AngelScript::asIScriptEngine* engine) {
      // The doc registry outlives one engine and Emplace only ever appends, so a second registration
      // into it in the same process would leave every @param of every binding written twice. Every
      // engine registers the same bindings, so what is dropped here is exactly what is written again
      // below. The builders are the only thing that writes into it, and none survives its call.
      util::ClearDocs();

      util::RegisterAddOns(engine);
      bindings::log::RegisterLog(engine);
      bindings::coro::RegisterCoro(engine);
      bindings::game_task::RegisterGameTask(engine);
      bindings::lifecycle::RegisterLifecycle(engine);
      bindings::mutex::RegisterMutex(engine);
      bindings::native_types::Register(engine);
      bindings::value::RegisterValue(engine);
      bindings::option::RegisterOption(engine);
      // After the option binding, and not before it: the GUI's `Submenu::AddOption` names `Option`, and
      // the two methods that give an option a UI callback of its own are registered onto the type the
      // option binding declared. The option binding knows nothing about the GUI and stays that way.
      bindings::gui::RegisterGui(engine);
      // Not part of the GUI despite being a thing the GUI shows: a notification is not a page, and the
      // namespace it lives in is a top-level one for that reason.
      bindings::notify::RegisterNotify(engine);

      // Qualified as the global namespace, which is where the generated registration header declares it:
      // the rest of the natives are in base::menu::natives, and that name is the one this reaches for
      // from in here - so a bare `natives::` would look in the wrong namespace for this one function.
      ::natives::RegisterAngelScript(engine);
    }

    AngelScript::asIScriptEngine* InitEngine() {
      AngelScript::asIScriptEngine* engine = AngelScript::asCreateScriptEngine();

      const int r = engine->SetMessageCallback(AngelScript::asFUNCTION(MessageCallback), nullptr, AngelScript::asCALL_CDECL);
      if (r < 0) {
        LOG_ERROR("[AS] Failed to set message callback");
        engine->ShutDownAndRelease();
        throw std::runtime_error("Failed to set message callback");
      }

      SetEngineProperties(engine);

      // Before RegisterBindings, and so before any module is built: the JIT compiles a function the
      // moment its module finishes building, so it has to be attached first or the first script to
      // load is the one that misses out.
      asjit::Attach(engine);

      RegisterBindings(engine);

      return engine;
    }
  }

  Script::Script(const ScriptManifest& metadata)
    : module_{nullptr}, name_(metadata.GetName()), option_prefix_(metadata.GetOptionPrefix()) {
    engine_ = InitEngine();

    try {
      std::filesystem::path script_dir = metadata.GetPath();

      AngelScript::CScriptBuilder builder;
      builder.StartNewModule(engine_, metadata.GetName().c_str());
      builder.SetIncludeCallback(&IncludeCallback, &script_dir);

      builder.AddSectionFromFile(metadata.GetMainFile().string().c_str());

      // A section that does not compile still leaves a module behind, one with nothing callable in
      // it, so this is the only point at which the failure can be told apart from a script that
      // simply has no GameTick. Loading it anyway would push the failure to the first tick.
      if (builder.BuildModule() < 0) {
        throw std::runtime_error("Failed to build script module '" + metadata.GetName() + "'");
      }

      module_ = builder.GetModule();
      if (!module_) {
        throw std::runtime_error("No module was built for script '" + metadata.GetName() + "'");
      }

      init_ = GetInitFunction();
      game_init_ = GetGameInitFunction();
      game_tick_ = GetGameTickFunction();

      // Built here so that the member is never written again: GetState() reads it from the UI thread
      // while the game thread ticks it.
      runtime_context_ = std::make_unique<ScriptContext>(engine_);

      // Last, and only once the module is up: a script that never built has no lines to log, and
      // nothing of its should be left behind by the throw below - not a logger, and not the file it
      // opens.
      logger_ = util::CreateScriptLogger(name_);
    } catch (...) {
      // The engine is a raw pointer, so nothing else would release the half-built script.
      engine_->ShutDownAndRelease();
      engine_ = nullptr;
      module_ = nullptr;
      throw;
    }
  }

  Script::Script() : module_{nullptr} {
    engine_ = InitEngine();
  }

  Script::~Script() {
    // The options this script registered go first, and before the engine does: the registry holds them
    // strongly, so it is the only thing keeping them alive, and an option whose module has been
    // discarded can name a callback that is no longer there. Nothing else takes them out - a script
    // that unloads is not a script that unregisters itself - and every path a script can end on comes
    // through here, including one that failed to build and one the manager is being torn down with.
    //
    // An unnamed script is one that was never loaded, which is the shape the doc generator makes; it
    // owns nothing, and asking for the options of the empty name would match every option the menu
    // registered itself, which are all owned by no script at all.
    if (!name_.empty()) {
      // Everything the script put into the menu's own state goes, in one place, before the engine does.
      // All three hold what a script made strongly - an option, a page, a string - so they are the only
      // things keeping them alive, and all three are keyed by the name of the script that made them,
      // which is all that is still readable this far into the destruction. A script that unloads does
      // not unregister itself, so every path a script can end on comes through here: a failed build, a
      // reload, a request from the script's own tick, and the manager's own teardown.
      //
      // Each is null-guarded, so a harness with no renderer or no localization manager unloads cleanly.
      if (ui::script_gui::kSCRIPT_SUBMENUS) {
        // The page is taken out of the renderer's map and off its stack, which leaves whoever was
        // standing on it on the page underneath. Nothing of the script is called back while that
        // happens - see ScriptSubmenuRegistry::RemoveOwnedBy.
        ui::script_gui::kSCRIPT_SUBMENUS->RemoveOwnedBy(name_);
      }

      if (ui::localization::kMANAGER) {
        ui::localization::kMANAGER->RemoveScriptTranslationsOwnedBy(name_);
      }

      if (options::kOPTION_REGISTRY) {
        options::kOPTION_REGISTRY->RemoveOptionsOwnedBy(name_);
      }
    }

    // The runtime context goes next. A parked coroutine's stack still holds the VM frame it was
    // resumed from, so shutting the engine down underneath it would leave that frame pointing at
    // freed memory - see the note on ScriptContext.
    runtime_context_.reset();

    // The logger goes next. Taking it out of the registry is all this does: the lines it still has
    // queued carry copies of the .as names they were logged from rather than anything of the module,
    // and the queue holds a reference to the logger until it has written them out - so a script that
    // logs and unloads in the same tick keeps its last line.
    util::DestroyScriptLogger(logger_);
    logger_.reset();

    if (module_) {
      module_->Discard();
      module_ = nullptr;
    }

    if (engine_) {
      engine_->ShutDownAndRelease();
      engine_ = nullptr;
    }
  }

  ScriptContext Script::CreateContext() const {
    return ScriptContext(engine_);
  }

  AngelScript::asIScriptFunction* Script::GetFunctionByDecl(const std::string_view declaration) const {
    if (!module_) {
      return nullptr;
    }

    // The engine's lookup takes a C string, so the view has to become one for the call. A task asks
    // for this once, when it starts, so the copy is not worth avoiding.
    const std::string decl{declaration};
    return module_->GetFunctionByDecl(decl.c_str());
  }

  bool Script::IsValid() const {
    return engine_ != nullptr && module_ != nullptr;
  }

  void Script::Tick() {
    if (unload_requested_) {
      return;
    }

    try {
      if (!inits_started_) {
        // The one pass that starts anything: init() goes to the general thread here, and GameInit -
        // which is this thread's - gets its first turn below. Both are started at once rather than one
        // after the other, since neither waits on the other; what they are both in front of is
        // GameTick, and that is gated on the two of them having finished.
        StartInits();
      }

      if (!game_init_done_.load(std::memory_order_relaxed)) {
        // One turn per pass, exactly as GameTick gets, and nothing else this pass: a GameInit that
        // parks is resumed on the next tick, and GameTick does not start until it has ended.
        if (runtime_context_->IsRunning()) {
          runtime_context_->Tick();
        } else if (game_init_) {
          runtime_context_->Run(game_init_, true);
          // So the first turn of GameInit runs in the pass the script was loaded in rather than the
          // next one. Run() only prepares the coroutine - this is what executes it.
          runtime_context_->Tick();
        }

        // Read off the context rather than assumed, because a first turn that ran to the end has
        // finished the init and one that parked has not.
        if (!runtime_context_->IsRunning()) {
          game_init_done_.store(true, std::memory_order_relaxed);
        }

        return;
      }

      if (!general_init_done_.load(std::memory_order_acquire)) {
        // Still on the general thread, or still queued there. GameTick waits: the whole point of the
        // general init is that it is the script's setup, and a tick that started before it ended would
        // be reading globals it is still making. Nothing this pass does either - the wait costs the
        // game nothing, which is why the init is a coroutine on a thread that parks.
        return;
      }

      if (runtime_context_->IsRunning()) {
        runtime_context_->Tick();
        return;
      }

      if (game_tick_) {
        runtime_context_->Run(game_tick_, true);
        // As above: Run() starts the coroutine and does not execute it, so this is what gives the
        // first GameTick its turn - in the pass the script was loaded in rather than the next one.
        runtime_context_->Tick();
      }
    } catch (const std::exception& e) {
      // Only an engine-level failure gets here - a script raising an exception is reported by the
      // context and ends that script's coroutine, not this call. A script the engine cannot run at
      // all must not stay loaded, and must not take the game thread down with it.
      LOG_ERROR("[AS] Script '{}' failed to run: {}", name_, e.what());
      RequestUnload();

      if (kAS_SCRIPT_MANAGER) {
        // The status is the manager's business: being already gone is the outcome this asked for.
        static_cast<void>(kAS_SCRIPT_MANAGER->UnloadScript(name_));
      }
    }
  }

  void Script::StartInits() {
    inits_started_ = true;

    // Marked finished rather than left alone: what the flags gate is GameTick, and a script without an
    // init has nothing for it to wait for. Set here, on the game thread, before either is read.
    if (!game_init_) {
      game_init_done_.store(true, std::memory_order_relaxed);
    }

    if (!init_) {
      general_init_done_.store(true, std::memory_order_release);
      return;
    }

    QueueGeneralInit();
  }

  void Script::QueueGeneralInit() {
    auto* const executor = menu::script::kGENERAL_TASK_EXECUTOR;
    if (!executor) {
      // Nothing will ever be waiting on a thread that does not exist, so the init is skipped rather
      // than the script sitting at its first tick for the rest of its life. It is a fault worth
      // reporting: a menu with no general thread has no init() for anybody, and every script that has
      // one is quietly missing its setup.
      LOG_ERROR("[AS] Script '{}' has an init() but there is no general task executor to run it on; it is skipped", name_);
      general_init_done_.store(true, std::memory_order_release);
      return;
    }

    // The handle rather than a reference, and the same reasoning as the task bodies in as_game_task:
    // this runs on the general thread while the game thread can unload the script at any moment, so
    // the task holds the script up for as long as it runs and lets go of one that is already gone. It
    // is also the only thing that keeps the engine alive for the context below.
    //
    // The declaration rather than the function, for the same reason a queued task names one: what is
    // handed out here outlives nothing, and a declaration resolved again in the module of a script this
    // task is holding is a lookup that can only answer while that script is alive.
    const std::string declaration = "void init()";
    executor->QueueTask([declaration, self = weak_from_this()] {
      const std::shared_ptr<Script> script = self.lock();
      if (!script) {
        // Unloaded between the queueing and this first pass. Nothing of the script is left to run.
        return;
      }

      try {
        auto* const fn = script->GetFunctionByDecl(declaration);
        if (!fn) {
          // The module this was resolved against is still loaded, so the function it named is gone
          // from it. Nothing to run, and the gate below still has to open - see the flag below.
          LOG_ERROR("[AS] Script '{}' cannot find '{}' in its own module", script->name_, declaration);
        } else {
          // Reached only while the script is alive, so this is the engine that keeps fn valid as well.
          // Declared after `script`, so that it - and the coroutine parked in it - is destroyed before
          // the reference that would take the engine down with it: the general thread may not be the
          // one that shuts an engine down, and this ordering is what makes it not be.
          ScriptContext context(fn->GetEngine());
          context.Run(fn, true);
          // So the init starts in the pass it was queued in rather than the one after it.
          context.Tick();

          while (context.IsRunning() && !script->IsUnloadRequested()) {
            minicoropp::this_coro::yield();
            context.Tick();
          }
        }
      } catch (const std::exception& e) {
        // The same treatment a GameTick the engine cannot run gets, except that the unload is asked for
        // and not carried out: this is the general thread, and the engine's teardown stays the game
        // thread's - the manager only queues it.
        LOG_ERROR("[AS] Script '{}' failed to run init: {}", script->name_, e.what());
        script->RequestUnload();

        if (kAS_SCRIPT_MANAGER) {
          static_cast<void>(kAS_SCRIPT_MANAGER->UnloadScript(script->name_));
        }
      }

      // Set on every path, including the two that ran nothing: what it gates is GameTick, and a script
      // whose init could not run is one that is on its way out rather than one to wait on forever. The
      // release pairs with the acquire at the gate in Tick: what it publishes is everything the init
      // wrote into the script's globals, which the game thread is about to start reading.
      script->general_init_done_.store(true, std::memory_order_release);
    });
  }

  ScriptState Script::GetState() const {
    // Both ends of both inits, and no distinction between one still running and one not started:
    // either way the script is initialising rather than ticking.
    if (!game_init_done_.load(std::memory_order_relaxed) || !general_init_done_.load(std::memory_order_acquire)) {
      return ScriptState::kLoaded;
    }

    // A plain yield is not a suspension: a script that yields every tick is working, one parked on a
    // sleep or on a wake is waiting.
    if (runtime_context_->IsParked() && runtime_context_->GetLastWait() != ScriptContext::Wait::kTick) {
      return ScriptState::kSuspended;
    }

    return ScriptState::kRunning;
  }

  void Script::RequestUnload() {
    unload_requested_ = true;
  }

  bool Script::IsUnloadRequested() const {
    return unload_requested_;
  }

  std::string Script::GetOptionPrefix() const {
    return option_prefix_;
  }

  std::string Script::QualifyOptionName(const std::string_view name) const {
    if (option_prefix_.empty()) {
      return std::string(name);
    }

    std::string qualified = option_prefix_;
    qualified += kOPTION_NAME_SEPARATOR;
    qualified += name;
    return qualified;
  }

  void Script::DumpAsPredefined(const std::filesystem::path& path) {
    const Script script;
    util::GenerateScriptPredefined(script.engine_, path);
  }

  AngelScript::asIScriptFunction* Script::GetInitFunction() const {
    if (!module_) {
      return nullptr;
    }

    // The general one, and the one most scripts will have: it is where a script puts the setup that is
    // neither the game's nor a frame's - an option, a translation, a page, a file read - since it is
    // the only entry point that runs off the game thread and may therefore wait.
    const auto init = module_->GetFunctionByDecl("void init()");
    LOG_DEBUG_CONDITIONAL(!init, "init function not found in script module '{}'", module_->GetName());
    return init;
  }

  AngelScript::asIScriptFunction* Script::GetGameInitFunction() const {
    if (!module_) {
      return nullptr;
    }

    // Any entry point may be left out - a script whose work is all in queued tasks has none of the
    // three - so an absent one is not a fault to report at error level. It is still worth a line at
    // debug level, since a script that meant to have one and misspelled it looks the same from here.
    const auto init = module_->GetFunctionByDecl("void GameInit()");
    LOG_DEBUG_CONDITIONAL(!init, "GameInit function not found in script module '{}'", module_->GetName());
    return init;
  }

  AngelScript::asIScriptFunction* Script::GetGameTickFunction() const {
    if (!module_) {
      return nullptr;
    }

    const auto tick = module_->GetFunctionByDecl("void GameTick()");
    LOG_DEBUG_CONDITIONAL(!tick, "GameTick function not found in script module '{}'", module_->GetName());
    return tick;
  }
}
