//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>
#include <minicoropp/coroutine.hpp>

#include <chrono>
#include <memory>

namespace base::menu::as::script {
  /**
   * A single AngelScript execution, optionally suspended across game ticks.
   *
   * A script coroutine is a minicoropp coroutine that runs the script function one pass of Execute()
   * at a time, parked by mco_yield in between. Each pass has to *return* before the coroutine is
   * parked, and that is the whole reason for coro::suspend() and its friends: AngelScript keeps a
   * context on its per-thread active-context stack for as long as Execute() is in flight, and takes
   * it off again when Execute() returns, in that order. A context left in flight would sit on the
   * stack while another script ran and finished, and the other script's Execute() would take this
   * one's entry off instead - an assertion failure in Debug, a stale asGetActiveContext() in Release.
   *
   * What a script asked to wait for therefore travels back to this class, which is where the
   * coroutine that has to honour it is parked.
   */
  class ScriptContext {
  public:
    /**
     * What the script's last coro call asked for, held from the native that recorded it until
     * RunCoroutine carries it out.
     */
    enum class Wait {
      /// Nothing to honour; a context that suspended for some other reason resumes next tick.
      kNone,
      /// coro::yield() - resume on the next tick.
      kTick,
      /// coro::sleep() - resume once the duration has passed.
      kFor,
      /// coro::suspend() - stay parked until another script wakes the coroutine.
      kIndefinitely,
    };

    explicit ScriptContext(AngelScript::asIScriptEngine* engine);
    ~ScriptContext();

    ScriptContext(const ScriptContext&) = delete;
    ScriptContext(ScriptContext&&) = delete;
    ScriptContext& operator=(const ScriptContext&) = delete;
    ScriptContext& operator=(ScriptContext&&) = delete;

    /**
     * Prepare `func` and either run it to completion or start it as a coroutine. A coroutine does not
     * execute until Tick() is called and is never restarted once it has finished.
     */
    void Run(AngelScript::asIScriptFunction* func, bool run_as_coroutine = false);

    /**
     * Advance a coroutine started by Run. Returns false once it has finished, which includes a
     * failure; a finished coroutine is never ticked again.
     */
    bool Tick();

    /**
     * Record what the running script asked for and suspend its context, so that Execute() returns
     * and the context comes off the thread's active-context stack before anything else runs. The
     * coroutine is parked by RunCoroutine afterwards, where the recorded wait is what it honours.
     *
     * This is the only way a native is allowed to make its script wait: parking the coroutine from
     * the native would leave this context in flight across the wait, and a second script finishing
     * first would then take this one's entry off the stack instead of its own.
     */
    void Suspend(Wait kind, std::chrono::high_resolution_clock::duration duration = {});

    /**
     * The script whose coroutine is running, which Run() handed over as the coroutine's user data.
     * Null outside a coroutine, and outside a script altogether.
     */
    [[nodiscard]] static ScriptContext* Current();

    [[nodiscard]] bool IsRunning() const {
      return !done_;
    }

  private:
    void ExecuteToCompletion();
    void RunCoroutine();

    /**
     * Park the coroutine the way the last RequestWait asked. Does not return until the wait is over.
     */
    void ApplyWait();

    AngelScript::asIScriptEngine* engine_;
    AngelScript::asIScriptContext* context_;
    std::unique_ptr<minicoropp::Coroutine> coro_;
    Wait wait_{Wait::kNone};
    std::chrono::high_resolution_clock::duration wait_duration_{};
    bool done_{true};
  };
}
