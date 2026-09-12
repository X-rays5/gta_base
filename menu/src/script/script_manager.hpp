//
// Created by X-ray on 20/12/2025.
//

#pragma once

#include <ankerl/unordered_dense.h>
#include <base-common/win32/signal.hpp>

#include <atomic>
#include <cstdint>
#include <thread>

#include "script_base.hpp"

namespace base::menu::script {
  /**
   * How long the general thread waits between passes while it still has work.
   *
   * A task that suspends itself is resumed on a later pass, so the general loop cannot park until
   * there is nothing left to run at all; it keeps this cadence instead. It is a wait, not a sleep: a
   * task queued in the meantime notifies the signal and the pass happens at once.
   */
  inline constexpr std::int32_t kGENERAL_TICK_MS = 16;

  /**
   * Owns the hosts that are ticked, and the loops that tick them.
   *
   * Each `ScriptBase::Type` is ticked by someone else - the menu and the game tick theirs themselves -
   * except `General`, which has a thread of its own here. That thread is what gives work somewhere to
   * go that is neither the menu's nor the game's, and it parks on a signal while idle so existing
   * costs nothing.
   */
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    std::uint64_t AddScript(std::unique_ptr<ScriptBase> script);
    void RemoveScript(std::uint64_t script_id);

    void TickScripts(ScriptBase::Type type);

    /// How many hosts of `type` are registered. Used to decide whether the general loop has anything
    /// to do besides the task executor.
    [[nodiscard]] std::size_t GetScriptCount(ScriptBase::Type type) const;

  private:
    using script_entry_t = std::pair<std::uint64_t, std::unique_ptr<ScriptBase>>;
    using script_list_t = std::vector<script_entry_t>;

    /**
     * The general thread's loop: tick the `General` hosts, then park or wait out the cadence.
     *
     * While the executor has a task left - including one that suspended itself - there is work to do
     * on the next pass, so it waits `kGENERAL_TICK_MS` for it. With the queue empty and no other host
     * of this type, there is nothing to do at all and it waits indefinitely; a queued task notifies
     * the signal and the next pass starts immediately.
     */
    void GeneralLoop();

    std::atomic_uint64_t script_id_counter_{0};
    ankerl::unordered_dense::map<ScriptBase::Type, script_list_t> scripts;

    win32::Signal general_signal_;
    std::thread general_thread_;
    std::atomic<bool> general_stopping_{false};
  };
  inline ScriptManager* kSCRIPT_MANAGER{};
}
