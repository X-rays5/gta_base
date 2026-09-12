//
// Created by X-ray on 12/09/2026.
//

#include "as_coro.hpp"

#include "../script/as_script_context.hpp"
#include "../util/as_bind.hpp"

#include <minicoropp/coroutine.hpp>

#include <chrono>
#include <cstdint>

#undef Yield

// The script-facing `coro` namespace, backed by minicoropp::this_coro, mirroring what the Lua side
// exposes as the `coro` table.
//
// The waiting calls do not park the coroutine themselves. They suspend the AngelScript context and
// record what was asked for on the ScriptContext running the script, which parks the coroutine once
// Execute() has returned. Parking from here would leave the context in flight across the wait, on
// the thread's active-context stack, where a second script finishing would take its entry off - see
// the note on ScriptContext for why that matters.
//
// Two of this_coro's functions are deliberately not bound. get_data() is the user data the host
// passed when the coroutine was created - the as::script::ScriptContext running the script - which a
// script has no business seeing, though that is exactly how these natives reach it. sleep_until(),
// and with it sleep_for()'s duration templates, need a clock type, and a script has no spelling for
// one; a millisecond count covers the same ground without inventing a type for it.
namespace base::menu::as::bindings::coro {
  namespace {
    using script::ScriptContext;

    // A coroutine handle is opaque to the script: the only one it can ever hold is a pointer
    // coro::handle() handed out, and its only use is handing it back to coro::wake(). Giving it a
    // type of its own rather than passing the handle as an int keeps it out of reach of arithmetic
    // and of the natives that take numbers.
    struct CoroHandle;

    // Every one of these is a no-op outside a coroutine, which matters because a script's GameInit
    // runs to completion rather than on one. yield() and suspend() would silently do nothing and let
    // the script carry on, and sleep_for() would block the game thread outright - a script asking to
    // wait has to be told it cannot rather than have the frame stall underneath it.
    bool OutsideCoroutine(const char* name) {
      if (minicoropp::this_coro::get_current_handle()) {
        return false;
      }

      LOG_ERROR("[AS] coro::{} called outside of a coroutine, which has no effect", name);
      return true;
    }

    // The script was handed to the coroutine as its user data by ScriptContext::Run, which is the one
    // use this_coro's get_data() has here - and the reason it is not bound for scripts. The guard
    // above means it is never null by the time these reach it.
    void Yield() {
      if (OutsideCoroutine("yield")) {
        return;
      }

      ScriptContext::Current()->Suspend(ScriptContext::Wait::kTick);
    }

    void SleepFor(const std::uint32_t ms) {
      if (OutsideCoroutine("sleep")) {
        return;
      }

      ScriptContext::Current()->Suspend(ScriptContext::Wait::kFor, std::chrono::milliseconds(ms));
    }

    void Suspend() {
      if (OutsideCoroutine("suspend")) {
        return;
      }

      ScriptContext::Current()->Suspend(ScriptContext::Wait::kIndefinitely);
    }

    // Neither of these runs the current coroutine, so neither is guarded: waking another script's
    // coroutine is the point of the pair, and the handle of a script that is not in one is simply
    // null.
    CoroHandle* CurrentHandle() {
      return static_cast<CoroHandle*>(minicoropp::this_coro::get_current_handle());
    }

    void Wake(CoroHandle* handle) {
      minicoropp::this_coro::wake_handle(handle);
    }
  }

  void RegisterCoro(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("coro");

    util::RegisterObjectType(engine, "Handle", 0, AngelScript::asOBJ_REF | AngelScript::asOBJ_NOCOUNT)
      .Desc("An opaque handle to a script coroutine. coro::handle() is the only thing that produces "
            "one, and coro::wake() the only thing that takes one.");

    // Qualified rather than the bare asFUNCTION macro, which expands to an unqualified asFunctionPtr:
    // that only resolves when one of the arguments drags the AngelScript namespace in through ADL,
    // which a thunk taking asIScriptGeneric* does and these - taking a uint or a handle - do not.
    util::RegisterGlobalFunction(engine, "void yield()", AngelScript::asFUNCTION(Yield), AngelScript::asCALL_CDECL)
      .Desc("Suspends the script until the next tick, so the game keeps running while it waits. This "
            "is the cheapest way to spread work over several ticks.");

    util::RegisterGlobalFunction(engine, "void sleep(uint ms)", AngelScript::asFUNCTION(SleepFor), AngelScript::asCALL_CDECL)
      .Desc("Suspends the script for a number of milliseconds. The script is not resumed until the "
            "time has passed, so waiting costs nothing.")
      .Param("ms", "How long to wait in milliseconds. The script resumes on the first tick after it.");

    util::RegisterGlobalFunction(engine, "void suspend()", AngelScript::asFUNCTION(Suspend), AngelScript::asCALL_CDECL)
      .Desc("Suspends the script until another coroutine wakes it with coro::wake(). It stays "
            "suspended forever if nothing ever does.");

    util::RegisterGlobalFunction(engine, "Handle@ handle()", AngelScript::asFUNCTION(CurrentHandle), AngelScript::asCALL_CDECL)
      .Desc("The handle of the running coroutine, or null outside of one.")
      .Returns("A handle another script can pass to coro::wake().");

    util::RegisterGlobalFunction(engine, "void wake(Handle@ handle)", AngelScript::asFUNCTION(Wake), AngelScript::asCALL_CDECL)
      .Desc("Wakes a suspended coroutine. It is the only thing that ends a coro::suspend(), and it "
            "also cuts a coro::sleep() short, as both are parked on the same deadline. A coroutine "
            "waiting on the next tick is unaffected, since it has no deadline to clear.")
      .Param("handle", "The handle of the coroutine to wake, from coro::handle().");

    engine->SetDefaultNamespace("");
  }
}
