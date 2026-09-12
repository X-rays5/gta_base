//
// Created by X-ray on 12/09/2026.
//

#include "as_script_context.hpp"

#include "../util/as_exception.hpp"

#include <ankerl/unordered_dense.h>

#include <chrono>
#include <cstddef>
#include <exception>
#include <mutex>
#include <stdexcept>
#include <utility>

namespace base::menu::as::script {
  namespace {
    // A suspended script keeps the AngelScript VM frame for Execute() on the coroutine's stack along
    // with everything the script has put there since it started, so this is not the size a coroutine
    // that only runs a callback would need. It is fixed rather than a constructor argument because a
    // script cannot choose it anyway; a script that overruns it reports kSTACK_OVERFLOW, as below.
    constexpr std::size_t kCoroutineStackSize = 256 * 1024;

    void LogException(AngelScript::asIScriptContext* context) {
      const util::AngelScriptException exception(context);
      LOG_ERROR("[AS] Script '{}' threw in {} at {}:{}: {}", exception.script_name(), exception.function_name(),
                exception.file_name(), exception.line_number(), exception.exception_msg());
    }

    /**
     * The coroutine handles that are live right now, which is the set of coroutines that have started
     * and not yet finished.
     *
     * A handle is a bare pointer into the coroutine it names, and thread::wake() writes through it, so a
     * handle that outlived its coroutine is a write into freed memory. That is reachable as soon as a
     * script can be unloaded while another script still holds its handle, which is why this exists:
     * handles are registered by the coroutine that owns them, and anything taking one has to ask first.
     *
     * The pointer may be handed out again by a later coroutine, which would read as live - waking that
     * one is a coroutine resuming early rather than a write into nothing, which is as far as a raw
     * pointer can be told apart.
     */
    std::mutex g_live_handles_mutex;
    ankerl::unordered_dense::set<const void*> g_live_handles;

    void RegisterLiveHandle(const void* handle) {
      const std::scoped_lock lock(g_live_handles_mutex);
      g_live_handles.insert(handle);
    }

    void UnregisterLiveHandle(const void* handle) {
      const std::scoped_lock lock(g_live_handles_mutex);
      g_live_handles.erase(handle);
    }
  }

  ScriptContext::ScriptContext(AngelScript::asIScriptEngine* engine) : engine_(engine), context_(nullptr) {
    context_ = engine_->CreateContext();
    if (!context_) {
      throw std::runtime_error("Failed to create script context");
    }
  }

  ScriptContext::~ScriptContext() {
    // The coroutine goes first. A suspended coroutine's stack still holds the VM frame it was
    // resumed from, so releasing the context underneath it would leave that frame pointing at freed
    // memory for as long as the coroutine was still alive.
    ResetCoroutine();

    if (context_) {
      const int r = context_->Release();
      if (r < 0) {
        LOG_ERROR("[AS] Failed to release script context");
      }

      context_ = nullptr;
      AngelScript::asThreadCleanup();
    }
  }

  void ScriptContext::Run(AngelScript::asIScriptFunction* func, const bool run_as_coroutine) {
    if (!func) {
      throw std::invalid_argument("Function pointer is null");
    }

    if (IsRunning()) {
      throw std::runtime_error("Script context is already running a function");
    }

    const int r = context_->Prepare(func);
    if (r < 0) {
      throw std::runtime_error("Failed to prepare script context");
    }

    if (!run_as_coroutine) {
      ExecuteToCompletion();
      return;
    }

    done_ = false;
    // The running context is handed over as the coroutine's user data, which is what
    // this_coro::get_data() reports for any native that needs to know whose script it is serving.
    ResetCoroutine();
    coro_ = std::make_unique<minicoropp::Coroutine>([this] { RunCoroutine(); }, this, kCoroutineStackSize);
  }

  void ScriptContext::ResetCoroutine() {
    if (live_handle_) {
      // A coroutine that is still parked is destroyed by minicoropp without unwinding its stack, so
      // this is the only thing that can take its handle out of the live set - see RunCoroutine.
      UnregisterLiveHandle(live_handle_);
      live_handle_ = nullptr;
    }

    coro_.reset();
  }

  bool ScriptContext::Tick() {
    if (done_ || !coro_) {
      return false;
    }

    const minicoropp::CoroResult result = coro_->resume();

    // kSUCCESS is what a plain yield reports as well as a body that ran to the end, and kYIELDING is
    // a thread::sleep() whose deadline has not passed yet. Neither is the end of the script on its own,
    // which is what done_ is for.
    if (result == minicoropp::CoroResult::kSUCCESS || result == minicoropp::CoroResult::kYIELDING) {
      return !done_;
    }

    done_ = true;

    if (result == minicoropp::CoroResult::kEXCEPTION) {
      try {
        std::rethrow_exception(coro_->exception());
      } catch (const std::exception& e) {
        LOG_ERROR("[AS] Script coroutine failed: {}", e.what());
      } catch (...) {
        LOG_ERROR("[AS] Script coroutine failed with an unknown exception");
      }
    } else {
      LOG_ERROR("[AS] Script coroutine failed with minicoropp result {}", static_cast<int>(result));
    }

    return false;
  }

  void ScriptContext::ExecuteToCompletion() {
    const int r = context_->Execute();
    if (r == AngelScript::asEXECUTION_FINISHED) {
      return;
    }

    // A script fault is reported rather than thrown: the engine's message callback has already logged
    // the location, and a script that throws must not take the game down with it. Only a failure that
    // says the engine itself could not run the function is unexpected enough to throw.
    if (r == AngelScript::asEXECUTION_EXCEPTION) {
      LogException(context_);
      return;
    }

    throw std::runtime_error("Failed to execute script function");
  }

  void ScriptContext::Suspend(const Wait kind, const std::chrono::high_resolution_clock::duration duration) {
    wait_ = kind;
    wait_duration_ = duration;
    // Kept past the point ApplyWait exchanges wait_ away, so that a parked script can still say what
    // it is parked on.
    last_wait_ = kind;

    // Suspend the context rather than the coroutine. The native that got here is running inside
    // Execute(), so this is what makes that call return - and RunCoroutine is what parks the
    // coroutine, once it has.
    if (AngelScript::asIScriptContext* context = AngelScript::asGetActiveContext()) {
      context->Suspend();
    }
  }

  ScriptContext* ScriptContext::Current() {
    return static_cast<ScriptContext*>(minicoropp::this_coro::get_data());
  }

  bool ScriptContext::IsCoroutineHandleLive(const void* handle) {
    if (!handle) {
      return false;
    }

    const std::scoped_lock lock(g_live_handles_mutex);
    return g_live_handles.contains(handle);
  }

  void ScriptContext::ApplyWait() {
    // Taken before parking, so the next suspension of the context - by whatever asked for it - starts
    // from a clean slate rather than inheriting this wait.
    const Wait kind = std::exchange(wait_, Wait::kNone);

    if (kind == Wait::kFor) {
      minicoropp::this_coro::sleep_for(wait_duration_);
      return;
    }

    if (kind == Wait::kIndefinitely) {
      minicoropp::this_coro::suspend_indefinitely();
      return;
    }

    // A tick wait, and a context that suspended without recording one. Neither sets a deadline, so
    // the coroutine runs again on the next tick.
    minicoropp::this_coro::yield();
  }

  void ScriptContext::RunCoroutine() {
    // This coroutine's handle is live from here until it is released below, which is as long as a
    // script holding it can expect thread::wake() to reach anything.
    //
    // Registered here and released by ResetCoroutine rather than by a guard in this frame, because
    // minicoropp destroys a suspended coroutine with mco_destroy, which frees the stack without
    // unwinding it: a guard here would never be destructed when a script is unloaded while parked,
    // and that is the case a stale handle is reached through.
    live_handle_ = minicoropp::this_coro::get_current_handle();
    if (live_handle_) {
      RegisterLiveHandle(live_handle_);
    }

    int r = context_->Execute();

    // One pass of Execute() per turn, still following the loop in the engine's own coroutine sample.
    // The coroutine is parked by ApplyWait, which is what keeps a suspended context off the thread's
    // active-context stack for as long as it waits - see the note on ScriptContext.
    //
    // parked_ brackets the park rather than the yield, so it stays true across the turns where
    // Resume() reports kYIELDING without entering the coroutine again, which is exactly the stretch a
    // script is waiting through.
    while (r == AngelScript::asEXECUTION_SUSPENDED) {
      parked_ = true;
      ApplyWait();
      parked_ = false;
      r = context_->Execute();
    }

    if (r == AngelScript::asEXECUTION_EXCEPTION) {
      LogException(context_);
    } else if (r != AngelScript::asEXECUTION_FINISHED) {
      LOG_ERROR("[AS] Script coroutine stopped with execution result {}", r);
    }

    // The coroutine ran to its own end rather than being cut short, so its handle stops naming a live
    // coroutine now. A body that threw never reaches this, and leaves the release to ResetCoroutine.
    if (live_handle_) {
      UnregisterLiveHandle(live_handle_);
      live_handle_ = nullptr;
    }

    done_ = true;
  }
}
