//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include "as_script.hpp"
#include <ankerl/unordered_dense.h>

#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

namespace base::menu::as::script {
  /**
   * The scripts that are loaded, keyed by name.
   *
   * Loading and unloading come from the menu's thread pool and from the scripts themselves, while
   * ticking happens on the game thread, so the map is behind a lock and a script's engine is only
   * ever released by the game thread - see TickScripts.
   */
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    ScriptManager(const ScriptManager&) = delete;
    ScriptManager(ScriptManager&&) = delete;
    ScriptManager& operator=(const ScriptManager&) = delete;
    ScriptManager& operator=(ScriptManager&&) = delete;

    /**
     * Build the script described by `manifest` and keep it alive under its name. Returns a handle
     * that expires once the script is unloaded.
     */
    StatusOr<std::weak_ptr<Script>> LoadScript(const ScriptManifest& manifest);

    /**
     * The script registered under `name`. The handle expires once the script is unloaded.
     *
     * It is a handle to *observe* with, not to own: whoever locks it is then the one releasing the
     * engine if the script is unloaded while they hold it, and only the game thread may do that.
     * GetScriptState is the way to ask about a script without taking a reference to it.
     */
    StatusOr<std::weak_ptr<Script>> GetScript(const std::string& name);

    /**
     * Unload the script registered under `name`, which also stops it being ticked. The engine itself
     * is shut down by the game thread, on its next pass, so this is safe to call from any thread -
     * including from the script being unloaded.
     */
    Status UnloadScript(const std::string& name);

    /**
     * Handles to every loaded script, expired ones excluded.
     */
    std::vector<std::weak_ptr<Script>> GetAllScripts() const;

    /**
     * What `name` is doing, or kNotLoaded when no such script is loaded.
     */
    [[nodiscard]] ScriptState GetScriptState(const std::string& name) const;

    /**
     * One pass over the loaded scripts: the inits still to run, then a turn of GameTick.
     *
     * Game thread only. It is also the only place a script is destroyed, since an unloading thread
     * queues the script here rather than releasing it, and the pass holds its own references for as
     * long as it is running.
     */
    void TickScripts();

    /**
     * The name of the script running on this thread, or empty when none is. A script is identified by
     * the module it was built as, which Script names after the manifest - the same name the manager
     * keeps it under.
     */
    [[nodiscard]] static std::string RunningScriptName();

    static std::vector<ScriptManifest> GetScriptsOnDisk();

    /**
     * Writes all registered types and function to as.predefined file for use in the IDE.
     */
    static void WriteAsPredefined();

  private:
    [[nodiscard]] std::vector<std::shared_ptr<Script>> SnapshotScripts() const;

    /**
     * Release everything an unloading thread has queued. Called from TickScripts, and from the
     * destructor for whatever is left when nothing will tick again.
     */
    void DrainPendingDestruction();

  private:
    /**
     * Guards the map and the queue below. Shared for the reads a loader, a tick pass or the menu
     * makes, unique for a load, an unload and a drain. A script's own engine is not built under it.
     */
    mutable std::shared_mutex scripts_mutex_;

    /**
     * Shared rather than unique because the handles handed out are weak: loading a script must not
     * force its callers to keep the manager's own reference alive.
     */
    ankerl::unordered_dense::map<std::string, std::shared_ptr<Script>> scripts_{};

    /**
     * References to release on the game thread, oldest first: scripts that were unloaded, from
     * whichever thread asked. What they have in common is that releasing the last of a script's
     * engines may not happen where the request was made, and this is where it is parked until the
     * thread that may do it makes a pass.
     */
    std::vector<std::shared_ptr<Script>> pending_destruction_;

    /**
     * Serialises building scripts, which is not about the map at all: registering a script's bindings
     * writes to the process-global documentation registry, which is not thread-safe, and two loaders
     * on the pool would otherwise build into it at the same time.
     */
    std::mutex construction_mutex_;

    /// The tick host's id in the game's script manager, absent when there was no manager to register
    /// with - which is how a manager in a test is built.
    std::optional<std::uint64_t> host_id_;
  };

  inline ScriptManager* kAS_SCRIPT_MANAGER = nullptr;
}
