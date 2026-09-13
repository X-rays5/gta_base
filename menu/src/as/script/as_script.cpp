//
// Created by X-ray on 11/09/2026.
//

#include "as_script.hpp"
#include "as_script_manager.hpp"
#include <angelscript.h>
#include <asjit.hpp>
#include "../../natives/natives_as.hpp"
#include "../../options/option_registry.hpp"
#include "../../script/script_manager.hpp"
#include "../bindings/as_coro.hpp"
#include "../bindings/as_game_task.hpp"
#include "../bindings/as_lifecycle.hpp"
#include "../bindings/as_log.hpp"
#include "../bindings/as_mutex.hpp"
#include "../bindings/as_native_types.hpp"
#include "../bindings/as_option.hpp"
#include "../bindings/as_value.hpp"
#include "../util/as_bind.hpp"
#include "../util/as_generate_predefined.hpp"
#include "../util/as_script_logger.hpp"
#include "../util/as_util.hpp"
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <base-common/fs/vfs.hpp>

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
    if (!name_.empty() && options::kOPTION_REGISTRY) {
      options::kOPTION_REGISTRY->RemoveOptionsOwnedBy(name_);
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
      if (!init_done_) {
        if (game_init_) {
          // Runs to completion rather than as a coroutine: there is nothing for GameInit to wait on,
          // and it has to have finished before GameTick gets a turn.
          runtime_context_->Run(game_init_, false);
        }

        // Set even for a script with no GameInit: what it stands for is that the script has started,
        // which is the difference between the loaded and running states.
        init_done_ = true;

        // A script that unloaded itself from GameInit has no GameTick to start, and the engine it
        // just ran on is only still alive because this pass holds a reference to the script.
        if (unload_requested_) {
          return;
        }
      }

      if (runtime_context_->IsRunning()) {
        runtime_context_->Tick();
        return;
      }

      if (game_tick_) {
        runtime_context_->Run(game_tick_, true);
        // So the first GameTick runs in the pass the script was loaded in rather than the next one.
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

  ScriptState Script::GetState() const {
    if (!init_done_) {
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

  AngelScript::asIScriptFunction* Script::GetGameInitFunction() const {
    if (!module_) {
      return nullptr;
    }

    // Either entry point may be left out - a script whose work is all in queued tasks has neither -
    // so an absent one is not a fault to report at error level. It is still worth a line at debug
    // level, since a script that meant to have one and misspelled it looks the same from here.
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
