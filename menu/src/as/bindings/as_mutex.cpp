//
// Created by X-ray on 12/09/2026.
//

#include "as_mutex.hpp"

#include "../script/as_script_context.hpp"
#include "../util/as_bind.hpp"

#include <minicoropp/coroutine.hpp>
#include <minicoropp/mutex.hpp>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <new>
#include <queue>

// The script-facing `std::mutex` is minicoropp::Mutex, which is what makes it a script type at all
// rather than a lock the script would have to spin on: a script that locks a held mutex waits on the
// lock instead of on the clock, so waiting for it costs no frame time.
//
// The waiting is the one thing ScriptMutex does not take from minicoropp::Mutex. Its lock() parks
// the coroutine from inside itself, which here would mean parking while Execute() is still in flight
// and leaving this context on the thread's active-context stack while another script ran - see the
// note on ScriptContext. So the state stays minicoropp's (try_lock and unlock are its own) and only
// the queue and the parking are done here, through ScriptContext::Suspend.
//
// That split is why unlock() hands ownership over instead of releasing: a script parked through
// ScriptContext is not resumed inside lock(), it is resumed *after* it, so there is no point in
// lock() at which the wait can be checked again. The waiter is made the owner before it is woken,
// which is the same thing minicoropp::Mutex does by leaving its flag set across a handover.
namespace base::menu::as::bindings::mutex {
  namespace {
    using script::ScriptContext;

    /**
     * What a script actually holds. minicoropp::Mutex is non-copyable, non-movable and carries no
     * reference count of its own, and AngelScript holds a reference type through AddRef/Release
     * alone - so the script's handle is to this wrapper, which owns the mutex and counts the
     * handles. The lock methods keep their standard names, so RegisterMethod derives their
     * declarations from them and nothing has to be spelled twice.
     */
    class ScriptMutex {
    public:
      ScriptMutex() = default;

      ScriptMutex(const ScriptMutex&) = delete;
      ScriptMutex(ScriptMutex&&) = delete;
      ScriptMutex& operator=(const ScriptMutex&) = delete;
      ScriptMutex& operator=(ScriptMutex&&) = delete;

      void lock() {
        if (mutex_.try_lock()) {
          return;
        }

        // Contended, so this call would have to wait for another script to release it - and a script
        // that is not a coroutine has nothing to park. Blocking the thread instead would stall the
        // frame on a lock only a suspended script can ever release, so this is refused as loudly as
        // the coro:: calls are, and the lock is not taken.
        ScriptContext* script = ScriptContext::Current();
        if (!script) {
          LOG_ERROR("[AS] std::mutex::lock called outside of a coroutine cannot wait for a held mutex, which has no effect");
          return;
        }

        void* const self = minicoropp::this_coro::get_current_handle();

        {
          std::lock_guard guard(queue_mtx_);
          // Tried again under the queue's lock: the holder may have released it since the attempt
          // above, and queueing for a mutex that is free by now would park this script until
          // something unlocked a lock that nobody holds.
          if (mutex_.try_lock()) {
            return;
          }
          wait_queue_.push(self);
        }

        // Nothing may follow this. Suspend() only sets a flag on the context - the VM honours it when
        // this native returns, so code written after it would run straight away rather than when the
        // script is resumed, and a retry loop here would spin instead of waiting. The next statement
        // of the calling script runs only once the script has been resumed, by which time unlock()
        // has handed this waiter the lock.
        script->Suspend(ScriptContext::Wait::kIndefinitely);
      }

      bool try_lock() {
        return mutex_.try_lock();
      }

      void unlock() {
        void* next = nullptr;
        {
          std::lock_guard guard(queue_mtx_);
          if (!wait_queue_.empty()) {
            next = wait_queue_.front();
            wait_queue_.pop();
          }
        }

        if (next) {
          // The lock is not released, it is handed over: ownership goes to the waiter being woken,
          // and it is left locked so that no third script can take it in the gap before that waiter
          // is next ticked.
          minicoropp::this_coro::wake_handle(next);
          return;
        }

        mutex_.unlock();
      }

      void AddRef() {
        refs_.fetch_add(1, std::memory_order_relaxed);
      }

      void Release() {
        if (refs_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
          delete this;
        }
      }

    private:
      // Starts at one for the reference the factory hands to AngelScript, which is the reference the
      // first handle to take ownership of it takes over.
      std::atomic<std::uint32_t> refs_{1};
      minicoropp::Mutex mutex_;

      // Guards the queue and orders it against mutex_'s own state, and is always taken before
      // mutex_'s internal lock rather than the other way around.
      std::mutex queue_mtx_;
      /// The coroutine handles waiting for the lock, oldest first. Only non-empty while mutex_ is
      /// locked, since unlock() empties it a waiter at a time.
      std::queue<void*> wait_queue_;
    };

    ScriptMutex* Create() {
      return new ScriptMutex();
    }

    void AddRef(ScriptMutex* self) {
      self->AddRef();
    }

    void Release(ScriptMutex* self) {
      self->Release();
    }
  }

  void RegisterMutex(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("std");

    // No asOBJ_NOCOUNT: the script's handle owns the mutex, and AngelScript has to be able to tell
    // when the last one goes away.
    util::RegisterObjectType(engine, "mutex", 0, AngelScript::asOBJ_REF)
      .Desc("A mutex. Locking one that is already locked suspends the script until the holder "
            "unlocks it, so waiting for it costs no frame time.");

    // Spelled out rather than run through the helpers: the declaration of a behaviour is a fixed
    // shape that has nothing to do with the thunk behind it - "void f()" with the object first, and
    // a factory taking no arguments and returning a handle to the object it built.
    util::RegisterObjectBehaviour(engine, "mutex", AngelScript::asBEHAVE_FACTORY, "mutex@ f()", AngelScript::asFUNCTION(Create),
                                  AngelScript::asCALL_CDECL);
    util::RegisterObjectBehaviour(engine, "mutex", AngelScript::asBEHAVE_ADDREF, "void f()", AngelScript::asFUNCTION(AddRef),
                                  AngelScript::asCALL_CDECL_OBJFIRST);
    util::RegisterObjectBehaviour(engine, "mutex", AngelScript::asBEHAVE_RELEASE, "void f()", AngelScript::asFUNCTION(Release),
                                  AngelScript::asCALL_CDECL_OBJFIRST);

    util::RegisterMethod<&ScriptMutex::lock>(engine, "mutex", "lock")
      .Desc("Locks the mutex. If another script holds it, this one waits - and nothing else in this "
            "script runs until the holder unlocks. Waiting is only possible in a coroutine, since a "
            "script that does not have one has nothing to park; locking a held mutex from GameInit "
            "logs an error and returns without the lock rather than stalling the frame.")
      .Returns("Nothing. The mutex is held once it returns.");
    util::RegisterMethod<&ScriptMutex::try_lock>(engine, "mutex", "try_lock")
      .Desc("Locks the mutex only if it is free. Never waits.")
      .Returns("True when the lock was taken, false when another script already holds it.");
    util::RegisterMethod<&ScriptMutex::unlock>(engine, "mutex", "unlock")
      .Desc("Unlocks the mutex, handing it straight to the next script waiting for it.")
      .Returns("Nothing. Unlocking a mutex this script does not hold is undefined.");

    engine->SetDefaultNamespace("");
  }
}
