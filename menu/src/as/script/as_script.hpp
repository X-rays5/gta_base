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
   * What a script's own options are set apart from everything else by: the separator between a
   * script's prefix and the name of one of its options, as in `optreg.set_level`.
   */
  inline constexpr std::string_view kOPTION_NAME_SEPARATOR = ".";

  /**
   * Where a loaded script stands, as the UI reports it.
   */
  enum class ScriptState {
    /// Not in the manager at all: never loaded, or unloaded again since.
    kNotLoaded,
    /// Loaded and built, with its inits still to finish: GameTick has not started.
    kLoaded,
    /// Being ticked: the GameTick coroutine runs, or awaits the next tick.
    kRunning,
    /// Parked on a thread::sleep() or a thread::suspend() rather than merely waiting for the next tick.
    kSuspended,
  };

  class Script : public std::enable_shared_from_this<Script> {
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
     * Advance the script by one game tick, which is one turn of whichever of its three calls is
     * currently the script's business.
     *
     * The script has three entry points, and this is the game thread's end of all three:
     *
     * - `init()`, which runs on the general thread as a task rather than here - it is queued once, on
     *   the first tick, and the general thread drives it from there to the end.
     * - `GameInit()`, a coroutine on this thread.
     * - `GameTick()`, a coroutine on this thread, which does not start until both inits have finished
     *   or were skipped for not existing.
     *
     * One turn per pass, and only one thing running at a time: the two game-thread calls share a single
     * ScriptContext, and this only ever starts one when that context reports it is not running, so a
     * second GameTick cannot be started on top of a parked first one, and GameTick cannot start on top
     * of a GameInit that has not ended. init() is the general thread's and so is genuinely concurrent
     * with this - which is why GameTick waits for its completion flag rather than for the call.
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

    /**
     * The prefix this script's own options are registered under, as the manifest gives it - see
     * ScriptManifest::GetOptionPrefix. Empty for a script that has none to make one from.
     */
    [[nodiscard]] std::string GetOptionPrefix() const;

    /**
     * `name` as this script's options are registered: the prefix, the separator, and the name as the
     * script wrote it. The name is returned unchanged when this script has no prefix.
     *
     * Nothing here works out whether `name` is already qualified: a name that is not this script's own
     * is meant to be read as written, and the caller is the one that knows which of the two readings
     * it is after - see how Options::Find and Options::Run resolve one.
     */
    [[nodiscard]] std::string QualifyOptionName(std::string_view name) const;

  protected:
    explicit Script(const ScriptManifest& metadata);

    static void DumpAsPredefined(const std::filesystem::path& path);

  private:
    Script();

    /**
     * Start the two inits, once, on the first tick: `init()` is queued onto the general thread, and
     * GameInit needs nothing here - the pass that called this goes on to give it its first turn.
     *
     * A script without one of them has that one marked finished straight away, which is what "skipped
     * for not existing" means to the gate in Tick. Neither thread is told anything: what the flags are
     * read from is this class, and the one that runs on the general thread is set there as it ends.
     */
    void StartInits();

    /// Queue `init()` onto the general thread as a task, which drives it to the end and marks it done.
    void QueueGeneralInit();

    AngelScript::asIScriptFunction* GetInitFunction() const;
    AngelScript::asIScriptFunction* GetGameInitFunction() const;
    AngelScript::asIScriptFunction* GetGameTickFunction() const;

  private:
    AngelScript::asIScriptEngine* engine_;
    AngelScript::asIScriptModule* module_;
    std::string name_;

    /**
     * Resolved once, in the constructor, from the manifest: an option a script registers carries this
     * from the moment it is made, so it is read there rather than asked of a manifest the engine has
     * long outlived - a script's options are registered and run from inside script code.
     */
    std::string option_prefix_;

    /**
     * Resolved once, in the constructor, so that a module which never built cannot be dereferenced
     * on the first tick - and so that the tick pass reads nothing a loader is still writing.
     */
    AngelScript::asIScriptFunction* init_{nullptr};
    AngelScript::asIScriptFunction* game_init_{nullptr};
    AngelScript::asIScriptFunction* game_tick_{nullptr};

    /**
     * Created during construction rather than on the first tick: the pointer is then never written
     * again, so GetState() can read it from the UI thread.
     */
    std::unique_ptr<ScriptContext> runtime_context_;

    /**
     * Whether the inits have been started, so that the first tick starts them and no later one does.
     * The game thread's alone: Tick is the only thing that reads or writes it.
     */
    bool inits_started_{false};

    /**
     * Whether the general thread's `init()` is over - set there by the task that ran it, and here on
     * the first tick for a script that has no such function. Atomic because the two ends are on
     * different threads with nothing ordering them but this flag.
     */
    std::atomic<bool> general_init_done_{false};

    /// Whether GameInit is over, or was skipped for not existing. The game thread's, but read from
    /// this class's accessors - GetState is one - so it is atomic like the other.
    std::atomic<bool> game_init_done_{false};

    std::atomic<bool> unload_requested_{false};

    /**
     * The script's own logger, which is what its log::info and friends write to: logs/scripts/<name>/
     * <name>.log, and the console. Held for as long as the script is, and given back to spdlog on the
     * way out - see as_script_logger.hpp for what it does with a line a script logs.
     */
    std::shared_ptr<spdlog::logger> logger_;
  };
}
