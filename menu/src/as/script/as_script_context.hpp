//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <angelscript.h>
#include <minicoropp/coroutine.hpp>

#include <atomic>
#include <chrono>
#include <memory>

namespace base::menu::as::script {
  /**
   * A single AngelScript execution, optionally suspended across game ticks.
   *
   * A script coroutine is a minicoropp coroutine that runs the script function one pass of Execute()
   * at a time, parked by mco_yield in between. Each pass has to *return* before the coroutine is
   * parked, and that is the whole reason for thread::suspend() and its friends: AngelScript keeps a
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
     * What the script's last thread:: call asked for, held from the native that recorded it until
     * RunCoroutine carries it out.
     */
    enum class Wait {
      /// Nothing to honour; a context that suspended for some other reason resumes next tick
      kNone,
      /// thread::yield() - resume on the next tick.
      kTick,
      /// thread::sleep() - resume once the duration has passed.
      kFor,
      /// thread::suspend() - stay parked until another script wakes the coroutine.
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
     *
     * `object_argument` is the first parameter of `func` when there is one to hand over, and null when
     * the callee takes none. It is the *address of the variable holding the handle* rather than the
     * handle: a parameter declared as a reference receives where the value is, which is what the engine
     * itself pushes for one. Nothing here counts the reference either way - the caller owns the handle
     * it put in that variable and is the one that releases it, on every path including a callee that
     * throws - so a callee cannot be handed one this class would have to know how to give back.
     */
    void Run(AngelScript::asIScriptFunction* func, bool run_as_coroutine = false, void* object_argument = nullptr);

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

    /**
     * Whether `handle` names a coroutine that is still running. A handle is a bare pointer into the
     * coroutine it came from, so one a script kept hold of outlives it as soon as it ends - or as
     * soon as the script it belonged to is unloaded - and anything writing through it afterwards
     * would be writing into freed memory. thread::wake() is the one thing that does, and this is what
     * it checks first. Null is not live.
     */
    [[nodiscard]] static bool IsCoroutineHandleLive(const void* handle);

    [[nodiscard]] bool IsRunning() const {
      return !done_;
    }

    /**
     * Whether the coroutine is sitting in ApplyWait right now, which is what separates a script that
     * is waiting from one that is between ticks. Read from another thread to report the script's
     * state, so it is an atomic rather than the coroutine's own state, which may only be read from
     * the thread that resumes it.
     */
    [[nodiscard]] bool IsParked() const {
      return parked_.load(std::memory_order_relaxed);
    }

    /**
     * What the script last asked to wait for, valid while IsParked(). A plain thread::yield() is left
     * out of what counts as suspended: a script that yields every tick is working, not waiting.
     */
    [[nodiscard]] Wait GetLastWait() const {
      return last_wait_.load(std::memory_order_relaxed);
    }

  private:
    void ExecuteToCompletion();
    void RunCoroutine();

    /**
     * Release the coroutine, and its handle with it. The one place a coroutine is destroyed, because
     * a coroutine destroyed while parked has to take its handle out of the live set as it goes:
     * minicoropp frees a suspended coroutine's stack without unwinding it, so nothing inside the
     * coroutine can do that for itself.
     */
    void ResetCoroutine();

    /**
     * Park the coroutine the way the last RequestWait asked. Does not return until the wait is over.
     */
    void ApplyWait();

    AngelScript::asIScriptEngine* engine_;
    AngelScript::asIScriptContext* context_;
    std::unique_ptr<minicoropp::Coroutine> coro_;

    /// The handle of `coro_` while it names a live coroutine, null otherwise. Written only by the
    /// thread running the script, which is the same one that destroys it - see ResetCoroutine.
    const void* live_handle_{nullptr};

    Wait wait_{Wait::kNone};
    std::chrono::high_resolution_clock::duration wait_duration_{};
    bool done_{true};
    std::atomic<bool> parked_{false};
    std::atomic<Wait> last_wait_{Wait::kNone};
  };
}
