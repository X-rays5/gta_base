//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

#include "../../src/as/bindings/as_coro.hpp"
#include "../../src/as/bindings/as_mutex.hpp"
#include "../../src/as/script/as_script_context.hpp"
#include "../../src/as/util/as_bind.hpp"
#include "../../src/as/util/as_util.hpp"

#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

// The `coro` namespace and `std::mutex` both rest on one claim: a script that waits suspends its
// minicoropp coroutine and hands the frame straight back to the game, rather than blocking the tick
// it was resumed from. Nothing about that is visible in a compile, and the failure mode is a frozen
// game rather than a wrong value, so every test here asserts on the clock as well as on the script's
// progress. The mutex goes further than that: ownership is handed from one suspended coroutine to
// another by unlock(), so the last test drives two contexts against each other to pin it.
namespace {
  using base::menu::as::script::ScriptContext;

  std::uint32_t g_error_count = 0;
  std::vector<std::string> g_errors;

  /// Collects the first few errors rather than only the last, since one refused registration is
  /// rarely the only one and the count alone does not say which declaration the engine rejected.
  void CollectErrors(const AngelScript::asSMessageInfo* msg, void*) {
    if (msg->type != AngelScript::asMSGTYPE_ERROR) {
      return;
    }
    if (g_errors.size() < 5) {
      g_errors.push_back(std::string(msg->section ? msg->section : "") + ":" + std::to_string(msg->row) + " " + msg->message);
    }
    ++g_error_count;
  }

  std::string ReportedErrors() {
    std::string joined;
    for (const auto& error : g_errors) {
      joined += "\n  " + error;
    }
    return joined;
  }

  /// Where the scripts below leave the value that says how far they got.
  int g_progress = 0;

  /// A real engine with the two bindings under test, set up the way as_script.cpp registers them.
  AngelScript::asIScriptEngine* MakeEngine() {
    auto* engine = AngelScript::asCreateScriptEngine();
    EXPECT_NE(engine, nullptr);

    g_error_count = 0;
    g_errors.clear();
    g_progress = 0;
    engine->SetMessageCallback(AngelScript::asFUNCTION(CollectErrors), nullptr, AngelScript::asCALL_CDECL);

    base::menu::as::util::ClearDocs();
    base::menu::as::util::RegisterAddOns(engine);
    base::menu::as::bindings::coro::RegisterCoro(engine);
    base::menu::as::bindings::mutex::RegisterMutex(engine);
    base::menu::as::util::RegisterGlobalProperty(engine, "int g_progress", &g_progress);
    return engine;
  }

  /// Builds `source` into the engine's single test module and hands the module back. One module per
  /// engine, deliberately: GetModule with asGM_ALWAYS_CREATE discards the previous one, which would
  /// leave a function pointer taken from it dangling.
  AngelScript::asIScriptModule* Compile(AngelScript::asIScriptEngine* engine, const std::string& source) {
    AngelScript::asIScriptModule* mod = engine->GetModule("TestModule", AngelScript::asGM_ALWAYS_CREATE);
    mod->AddScriptSection("test.as", source.c_str());
    if (mod->Build() < 0) {
      ADD_FAILURE() << "the script did not build:\n" << source << ReportedErrors();
      return nullptr;
    }
    return mod;
  }

  /// A function of a module built by Compile. Null is a failure rather than a check for the caller,
  /// since every declaration asked for here is one the script is supposed to have.
  AngelScript::asIScriptFunction* Function(AngelScript::asIScriptModule* mod, const char* decl) {
    if (mod == nullptr) {
      return nullptr;
    }

    AngelScript::asIScriptFunction* func = mod->GetFunctionByDecl(decl);
    if (func == nullptr) {
      ADD_FAILURE() << decl << " was not compiled";
    }
    return func;
  }

  /// Wraps a body in the `main` the single-function tests all use.
  std::string Main(const std::string& body) {
    return "void main() {\n" + body + "\n}\n";
  }

  /// The one function of a single-function script, which is the shape most of these take.
  AngelScript::asIScriptFunction* CompileMain(AngelScript::asIScriptEngine* engine, const std::string& body) {
    return Function(Compile(engine, Main(body)), "void main()");
  }

  using Clock = std::chrono::steady_clock;

  /// Wall-clock milliseconds a call took, for the assertions that say a waiting call did not wait.
  template<typename F>
  long long TimedMs(F&& f) {
    const auto start = Clock::now();
    f();
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();
  }
}

// ---------------------------------------------------------------- what the namespaces expose

TEST(as_coro, both_namespaces_register_without_the_engine_refusing_anything) {
  auto* engine = MakeEngine();
  EXPECT_EQ(g_error_count, 0u) << ReportedErrors();
  engine->ShutDownAndRelease();
}

TEST(as_coro, the_waiting_calls_cost_nothing_outside_a_coroutine) {
  auto* engine = MakeEngine();

  // GameInit runs to completion rather than as a coroutine, so this is the shape every script starts
  // in. sleep() is the one that matters: outside a coroutine minicoropp falls back to
  // std::this_thread::sleep_for, which here would stall the game thread for the full second.
  auto* func = CompileMain(engine, "coro::yield(); coro::sleep(1000); coro::suspend(); g_progress = 1;");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  const long long elapsed = TimedMs([&] {
    ctx.Run(func, false);
  });

  EXPECT_EQ(g_progress, 1) << "a waiting call outside a coroutine returned without running the script on";
  EXPECT_LT(elapsed, 500) << "coro::sleep() blocked the caller's thread instead of having no effect";
  EXPECT_FALSE(ctx.IsRunning());

  engine->ShutDownAndRelease();
}

TEST(as_coro, handle_is_null_outside_a_coroutine) {
  auto* engine = MakeEngine();

  auto* func = CompileMain(engine, "coro::Handle@ h = coro::handle();\n"
                                   "if (h is null) { g_progress = 1; } else { g_progress = 2; }");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  ctx.Run(func, false);
  EXPECT_EQ(g_progress, 1);

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- coro::yield across ticks

TEST(as_coro, yield_spreads_the_script_over_ticks) {
  auto* engine = MakeEngine();

  auto* func = CompileMain(engine, "g_progress = 1; coro::yield(); g_progress = 2; coro::yield(); g_progress = 3;");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  ctx.Run(func, true);

  // A coroutine does not run until it is ticked, so nothing has happened yet.
  EXPECT_TRUE(ctx.IsRunning());
  EXPECT_EQ(g_progress, 0);

  EXPECT_TRUE(ctx.Tick());
  EXPECT_EQ(g_progress, 1);

  EXPECT_TRUE(ctx.Tick());
  EXPECT_EQ(g_progress, 2);

  // The last tick runs the script off its end, which is what makes Tick report it is done.
  EXPECT_FALSE(ctx.Tick());
  EXPECT_EQ(g_progress, 3);
  EXPECT_FALSE(ctx.IsRunning());

  // A finished coroutine is never resumed again.
  EXPECT_FALSE(ctx.Tick());
  EXPECT_EQ(g_progress, 3);

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- coro::sleep

TEST(as_coro, sleep_gates_the_script_on_the_clock) {
  auto* engine = MakeEngine();

  auto* func = CompileMain(engine, "g_progress = 1; coro::sleep(200); g_progress = 2;");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  ctx.Run(func, true);
  ASSERT_TRUE(ctx.Tick());
  ASSERT_EQ(g_progress, 1);

  // Ticking while the script is asleep has to return at once. If the coroutine were resumed anyway
  // and waited inside, this call would take the remaining 200ms and the game would stutter by it.
  const long long early = TimedMs([&] {
    EXPECT_TRUE(ctx.Tick());
  });
  EXPECT_LT(early, 100) << "the tick waited out a sleeping script instead of skipping it";
  EXPECT_EQ(g_progress, 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  EXPECT_FALSE(ctx.Tick());
  EXPECT_EQ(g_progress, 2);

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- coro::suspend and coro::wake

namespace {
  const char* kHandshakeScript = R"AS(
coro::Handle@ g_handle;

void Waiter() {
  @g_handle = coro::handle();
  g_progress = 1;
  coro::suspend();
  g_progress = 2;
}

void Waker() {
  g_progress = 3;
  coro::wake(g_handle);
}
)AS";
}

TEST(as_coro, suspend_waits_for_a_wake_from_another_script) {
  auto* engine = MakeEngine();

  auto* mod = Compile(engine, kHandshakeScript);
  auto* waiter = Function(mod, "void Waiter()");
  auto* waker = Function(mod, "void Waker()");
  ASSERT_NE(waiter, nullptr);
  ASSERT_NE(waker, nullptr);

  ScriptContext waiting(engine);
  ScriptContext waking(engine);

  waiting.Run(waiter, true);
  ASSERT_TRUE(waiting.Tick());
  ASSERT_EQ(g_progress, 1);

  // Nothing can wake it yet, so it stays suspended for as many ticks as pass.
  EXPECT_TRUE(waiting.Tick());
  EXPECT_TRUE(waiting.Tick());
  EXPECT_EQ(g_progress, 1);

  waking.Run(waker, true);
  EXPECT_FALSE(waking.Tick());
  EXPECT_EQ(g_progress, 3);

  // The handle the waiter handed out was woken, so the next tick of its own context is the one that
  // carries it past the suspend().
  EXPECT_FALSE(waiting.Tick());
  EXPECT_EQ(g_progress, 2);

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- std::mutex

TEST(as_mutex, try_lock_reports_ownership_without_waiting) {
  auto* engine = MakeEngine();

  auto* func = CompileMain(engine, "std::mutex@ m = std::mutex();\n"
                                   "g_progress = m.try_lock() ? 1 : 0;\n"
                                   "if (m.try_lock()) { g_progress += 10; }\n"
                                   "m.unlock();\n"
                                   "if (m.try_lock()) { g_progress += 100; }\n"
                                   "m.unlock();");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  ctx.Run(func, false);

  // First try takes it, the second must not, and after unlock it is free again.
  EXPECT_EQ(g_progress, 101);

  engine->ShutDownAndRelease();
}

// The one place std::mutex is deliberately not std::mutex: a script that is not a coroutine has
// nothing to park, so lock() cannot wait for it. Taking a free lock still has to work - GameInit
// locking an uncontended mutex is the ordinary case - and locking a held one is refused and logged
// rather than stalling the frame on a lock only a suspended script can release. This pins that
// choice; if it is ever changed, it should be changed deliberately.
TEST(as_mutex, lock_outside_a_coroutine_takes_a_free_mutex_but_never_waits) {
  auto* engine = MakeEngine();

  auto* func = CompileMain(engine, "std::mutex@ m = std::mutex();\n"
                                   "m.lock();\n"
                                   "g_progress = m.try_lock() ? 1 : 2;\n"
                                   "m.unlock();\n"
                                   "if (m.try_lock()) { g_progress += 100; }\n"
                                   "m.unlock();");
  ASSERT_NE(func, nullptr);

  ScriptContext ctx(engine);
  const long long elapsed = TimedMs([&] {
    ctx.Run(func, false);
  });

  EXPECT_LT(elapsed, 100);
  EXPECT_EQ(g_progress, 102) << "locking a free mutex outside a coroutine did not hold it";

  engine->ShutDownAndRelease();
}

namespace {
  /// A holder that parks on the lock across a tick, and a script that runs to completion against it -
  /// the shape a GameInit locking something another script holds takes.
  const char* kHeldAcrossTicksScript = R"AS(
std::mutex@ g_lock;

void Holder() {
  @g_lock = std::mutex();
  g_lock.lock();
  g_progress = 1;
  coro::yield();
  g_progress = 2;
  g_lock.unlock();
}

void Unparked() {
  g_progress = 10;
  g_lock.lock();
  g_progress = 11;
}
)AS";
}

TEST(as_mutex, lock_outside_a_coroutine_does_not_wait_for_a_held_mutex) {
  auto* engine = MakeEngine();

  auto* mod = Compile(engine, kHeldAcrossTicksScript);
  auto* holder = Function(mod, "void Holder()");
  auto* unparked = Function(mod, "void Unparked()");
  ASSERT_NE(holder, nullptr);
  ASSERT_NE(unparked, nullptr);

  ScriptContext holding(engine);
  holding.Run(holder, true);
  ASSERT_TRUE(holding.Tick());
  ASSERT_EQ(g_progress, 1) << "the holder did not take the lock";

  ScriptContext plain(engine);
  const long long elapsed = TimedMs([&] {
    plain.Run(unparked, false);
  });

  EXPECT_LT(elapsed, 100) << "locking a held mutex outside a coroutine stalled the frame";
  EXPECT_EQ(g_progress, 11) << "the script did not carry on past a lock it was refused";
  EXPECT_FALSE(plain.IsRunning());

  engine->ShutDownAndRelease();
}

namespace {
  /// Two scripts sharing one mutex. The holder takes it, yields once, and gives it up; the contender
  /// blocks on it in between. Every step is separated by a tick, so the order in which the two make
  /// progress is decided by the mutex and not by the scheduler.
  const char* kSharedMutexScript = R"AS(
std::mutex@ g_lock;

void Holder() {
  @g_lock = std::mutex();
  g_lock.lock();
  g_progress = 1;
  coro::yield();
  g_progress = 2;
  g_lock.unlock();
  g_progress = 3;
}

void Contender() {
  g_progress = 10;
  g_lock.lock();
  g_progress = 11;
  g_lock.unlock();
}
)AS";
}

TEST(as_mutex, locking_a_held_mutex_suspends_the_script_rather_than_the_thread) {
  auto* engine = MakeEngine();

  auto* mod = Compile(engine, kSharedMutexScript);
  auto* holder = Function(mod, "void Holder()");
  auto* contender = Function(mod, "void Contender()");
  ASSERT_NE(holder, nullptr);
  ASSERT_NE(contender, nullptr);

  ScriptContext holding(engine);
  ScriptContext waiting(engine);

  holding.Run(holder, true);
  ASSERT_TRUE(holding.Tick());
  ASSERT_EQ(g_progress, 1) << "the holder did not take the lock";

  waiting.Run(contender, true);
  const long long blocked = TimedMs([&] {
    EXPECT_TRUE(waiting.Tick());
  });
  EXPECT_LT(blocked, 100) << "locking a held mutex blocked the tick instead of suspending the script";
  EXPECT_EQ(g_progress, 10) << "the contender got past a lock the holder still owns";

  // Still held, so the contender is still parked - it is woken by unlock(), not by being ticked.
  EXPECT_TRUE(waiting.Tick());
  EXPECT_EQ(g_progress, 10);

  // The holder runs to its unlock(), which is where the lock changes hands.
  EXPECT_FALSE(holding.Tick());
  EXPECT_EQ(g_progress, 3);

  EXPECT_FALSE(waiting.Tick());
  EXPECT_EQ(g_progress, 11) << "the contender was not handed the lock on unlock()";

  engine->ShutDownAndRelease();
}

namespace {
  /// A holder and two scripts queued behind it. The second waiter is what makes this different from
  /// the test above: both waiters are parked on the lock at once, so the order they are woken in and
  /// the order they finish in are not necessarily the order they parked in.
  const char* kQueuedMutexScript = R"AS(
std::mutex@ g_lock;

void Holder() {
  @g_lock = std::mutex();
  g_lock.lock();
  coro::yield();
  g_lock.unlock();
}

void First() {
  g_lock.lock();
  g_progress += 10;
  g_lock.unlock();
}

void Second() {
  g_lock.lock();
  g_progress += 100;
  g_lock.unlock();
}
)AS";
}

TEST(as_mutex, a_second_waiter_does_not_disturb_the_first) {
  auto* engine = MakeEngine();

  auto* mod = Compile(engine, kQueuedMutexScript);
  auto* holder = Function(mod, "void Holder()");
  auto* first = Function(mod, "void First()");
  auto* second = Function(mod, "void Second()");
  ASSERT_NE(holder, nullptr);
  ASSERT_NE(first, nullptr);
  ASSERT_NE(second, nullptr);

  ScriptContext holding(engine);
  ScriptContext one(engine);
  ScriptContext two(engine);

  holding.Run(holder, true);
  ASSERT_TRUE(holding.Tick());

  // Both queue up on the lock, in this order.
  one.Run(first, true);
  ASSERT_TRUE(one.Tick());
  two.Run(second, true);
  ASSERT_TRUE(two.Tick());
  EXPECT_EQ(g_progress, 0) << "a waiter got past a lock the holder never released";

  // The holder's unlock hands the lock to the first waiter.
  EXPECT_FALSE(holding.Tick());

  EXPECT_FALSE(one.Tick());
  EXPECT_EQ(g_progress, 10) << "the first waiter was not handed the lock on unlock()";

  EXPECT_FALSE(two.Tick());
  EXPECT_EQ(g_progress, 110) << "the second waiter was not handed the lock on the next unlock()";

  engine->ShutDownAndRelease();
}
