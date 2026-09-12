//
// Created by X-ray on 11/09/2026.
//

#pragma once
#include <angelscript.h>
#include <atomic>
#include <memory>
#include <string>
#include <string_view>
#include "as_script_manifest.hpp"
#include "as_script_context.hpp"

namespace spdlog {
  class logger;
}

namespace base::menu::as::script {
  /**
   * Where a loaded script stands, as the UI reports it.
   */
  enum class ScriptState {
    /// Not in the manager at all: never loaded, or unloaded again since.
    kNotLoaded,
    /// Loaded and built, with GameInit not yet run.
    kLoaded,
    /// Being ticked: the GameTick coroutine runs, or awaits the next tick.
    kRunning,
    /// Parked on a thread::sleep() or a thread::suspend() rather than merely waiting for the next tick.
    kSuspended,
  };

  class Script {
    friend class ScriptManager;

  public:
    ~Script();

    Script(const Script&) = delete;
    Script(Script&&) = delete;
    Script& operator=(const Script&) = delete;
    Script& operator=(Script&&) = delete;

    ScriptContext CreateContext() const;

    /**
     * A function of the module by its declaration, or null when the module has none.
     *
     * This is how a queued task gets back to the function it was asked to run. It cannot hold that
     * function itself: the engine lends a callback to the native it is passed to for the length of
     * the call and no longer, so keeping the pointer past it would be a pointer into a function
     * nothing is holding a reference to any more. A declaration names one exactly - where a name
     * would also match an overload - and is resolved against a module the caller is keeping alive.
     */
    [[nodiscard]] AngelScript::asIScriptFunction* GetFunctionByDecl(std::string_view declaration) const;

    bool IsValid() const;

    /**
     * Advance the script by one game tick: GameInit, once, then the GameTick coroutine.
     *
     * Game thread only, and only while the manager still holds the script - which is what TickScripts
     * guarantees by ticking a snapshot of its own references.
     */
    void Tick();

    /**
     * What GetState() reports. Safe to read from any thread, which is what lets the menu show it
     * while the script runs.
     */
    [[nodiscard]] ScriptState GetState() const;

    /**
     * Stop the script. Nothing acts on this immediately: Tick() is what stops resuming it, so a
     * script that asked to unload is left alone from the next tick onwards. The flag is atomic
     * because the request and the tick belong to different threads.
     */
    void RequestUnload();

    [[nodiscard]] bool IsUnloadRequested() const;

  protected:
    explicit Script(const ScriptManifest& metadata);

    static void DumpAsPredefined(const std::filesystem::path& path);

  private:
    Script();

    AngelScript::asIScriptFunction* GetGameInitFunction() const;
    AngelScript::asIScriptFunction* GetGameTickFunction() const;

  private:
    AngelScript::asIScriptEngine* engine_;
    AngelScript::asIScriptModule* module_;
    std::string name_;

    /**
     * Resolved once, in the constructor, so that a module which never built cannot be dereferenced
     * on the first tick - and so that the tick pass reads nothing a loader is still writing.
     */
    AngelScript::asIScriptFunction* game_init_{nullptr};
    AngelScript::asIScriptFunction* game_tick_{nullptr};

    /**
     * Created during construction rather than on the first tick: the pointer is then never written
     * again, so GetState() can read it from the UI thread.
     */
    std::unique_ptr<ScriptContext> runtime_context_;
    std::atomic<bool> init_done_{false};
    std::atomic<bool> unload_requested_{false};

    /**
     * The script's own logger, which is what its log::info and friends write to: logs/scripts/<name>/
     * <name>.log, and the console. Held for as long as the script is, and given back to spdlog on the
     * way out - see as_script_logger.hpp for what it does with a line a script logs.
     */
    std::shared_ptr<spdlog::logger> logger_;
  };
}
