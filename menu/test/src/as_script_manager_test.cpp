//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

// The AngelScript headers are written against the menu's precompiled header: they name LOG_ERROR and
// Status without including either. A test is not that translation unit, so both are brought in here,
// as submenu_test does.
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

// A script's logger is reachable from anywhere by name, which is how the tests below read one back
// after the script that owned it is gone.
#include <spdlog/spdlog.h>

// Also before the menu's own headers, which declare things like std::filesystem::path without
// including them, for the same reason.
#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "../../src/as/bindings/as_coro.hpp"
#include "../../src/as/bindings/as_game_task.hpp"
#include "../../src/as/script/as_script.hpp"
#include "../../src/as/script/as_script_context.hpp"
#include "../../src/as/script/as_script_manager.hpp"
#include "../../src/as/script/as_script_manifest.hpp"
#include "../../src/as/util/as_bind.hpp"
#include "../../src/as/util/as_util.hpp"
#include "../../src/script/game_task_executor.hpp"
#include "../../src/script/general_task_executor.hpp"
#include "../../src/script/script_manager.hpp"

// The manager is the only place a script is loaded, ticked and unloaded, and it is where the two
// threads a script lives on meet: the menu's pool loads and unloads, and the game thread ticks. Its
// job is to make that meeting safe, so the tests are about what survives it - a script that unloads
// itself mid-tick, one that is unloaded while parked, a task that outlives nothing - and about the
// states the UI reads off the same map.
namespace {
  using base::menu::as::script::ScriptContext;
  using base::menu::as::script::ScriptManager;
  using base::menu::as::script::ScriptManifest;
  using base::menu::as::script::ScriptState;

  /// A scratch directory of scripts, removed with the test that made it. The manager reads scripts off
  /// the disk rather than out of the test, so a manifest and a main.as are what a test script is.
  ///
  /// Named randomly rather than after a counter: the suite is discovered into one ctest entry per test
  /// and ctest is free to run those in parallel, so the name has to be unique across processes.
  class ScriptDir {
  public:
    ScriptDir() {
      static std::atomic<int> counter{0};
      dir_ = std::filesystem::temp_directory_path() /
             ("as_script_manager_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1)));
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
      std::filesystem::create_directories(dir_);
    }

    ~ScriptDir() {
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
    }

    ScriptDir(const ScriptDir&) = delete;
    ScriptDir(ScriptDir&&) = delete;
    ScriptDir& operator=(const ScriptDir&) = delete;
    ScriptDir& operator=(ScriptDir&&) = delete;

    /// Writes `<dir_name>/manifest.toml` from `toml`, plus `<dir_name>/main.as` when `source` is
    /// given, and hands back the manifest read off the disk - which is how the menu gets one.
    ScriptManifest WriteManifest(const std::string& dir_name, const std::string& toml, const std::string& source = {}) const {
      const auto script_dir = dir_ / dir_name;
      std::filesystem::create_directories(script_dir);
      WriteFile(script_dir / "manifest.toml", toml);
      if (!source.empty()) {
        WriteFile(script_dir / "main.as", source);
      }

      ScriptManifest manifest(script_dir);
      const auto status = manifest.ReadMetaData();
      EXPECT_FALSE(status.has_error()) << "the test manifest did not read back: " << status.error().GetResultMessage();
      return manifest;
    }

    /// The ordinary case: a manifest naming `name` over a main.as holding `source`.
    ScriptManifest WriteScript(const std::string& name, const std::string& source) const {
      return WriteManifest(name, "name = \"" + name + "\"\nmain_file = \"main.as\"\n", source);
    }

  private:
    static void WriteFile(const std::filesystem::path& path, const std::string& contents) {
      std::ofstream file(path, std::ios::binary | std::ios::trunc);
      file << contents;
      EXPECT_TRUE(file.good()) << "could not write " << path.string();
    }

    std::filesystem::path dir_;
  };

  constexpr const char* kEmptyInit = R"AS(
void GameInit() {
}
)AS";

  /// Parks on the next tick rather than on anything else, which is what a working script does between
  /// frames: it stays running rather than being reported as suspended.
  constexpr const char* kYieldingTick = R"AS(
void GameTick() {
  thread::yield();
}
)AS";

  constexpr const char* kSuspendingTick = R"AS(
void GameTick() {
  thread::suspend();
}
)AS";

  /// GameInit's waiting calls have no coroutine to park, so it runs to its end either way; GameTick
  /// then runs as one, which the first tick carries to the yield and the second past it.
  constexpr const char* kYieldThenUnload = R"AS(
void GameInit() {
  thread::yield();
}

void GameTick() {
  thread::yield();
  script::unload();
}
)AS";

  constexpr const char* kUnloadFromInit = R"AS(
void GameInit() {
  script::unload();
}

void GameTick() {
}
)AS";

  constexpr const char* kUnloadFromTick = R"AS(
void GameTick() {
  script::unload();
}
)AS";

  /// A task is the script's other way onto the game thread, so unloading from one is the case that
  /// has to leave both the script and the task it is running in a state they can end in.
  constexpr const char* kTaskThatUnloads = R"AS(
void Task() {
  script::unload();
}

void GameInit() {
  thread::queue_game_task(@Task);
}
)AS";

  /// A task that yields twice and only then ends its script, so the turns the executor takes over it
  /// are visible in whether the script is still loaded.
  constexpr const char* kTaskThatYields = R"AS(
void Task() {
  thread::yield();
  thread::yield();
  script::unload();
}

void GameInit() {
  thread::queue_game_task(@Task);
}
)AS";

  /// Not a script at all: the module cannot be built, which has to be a load that failed rather than
  /// a script that sits there loaded and throws on its first tick.
  constexpr const char* kUnbuildable = R"AS(
void GameTick( {
  this is not AngelScript
}
)AS";
}

// ---------------------------------------------------------------- loading, getting, unloading

TEST(as_script_manager, a_loaded_script_can_be_got_back_by_name_and_unloaded) {
  const ScriptDir dir;
  ScriptManager manager;

  const auto loaded = manager.LoadScript(dir.WriteScript("hello", kEmptyInit));
  ASSERT_FALSE(loaded.has_error()) << loaded.error().GetResultMessage();

  const auto found = manager.GetScript("hello");
  ASSERT_FALSE(found.has_error()) << found.error().GetResultMessage();
  EXPECT_FALSE(found.value().expired());

  EXPECT_EQ(manager.GetAllScripts().size(), 1u);
  EXPECT_EQ(manager.GetScriptState("hello"), ScriptState::kLoaded) << "a script is loaded before it has been ticked";

  ASSERT_FALSE(manager.UnloadScript("hello").has_error());

  EXPECT_TRUE(manager.GetScript("hello").has_error());
  EXPECT_TRUE(manager.GetAllScripts().empty());
  EXPECT_EQ(manager.GetScriptState("hello"), ScriptState::kNotLoaded);
}

TEST(as_script_manager, the_same_name_cannot_be_loaded_twice) {
  const ScriptDir dir;
  ScriptManager manager;

  const auto manifest = dir.WriteScript("hello", kEmptyInit);
  ASSERT_FALSE(manager.LoadScript(manifest).has_error());

  const auto second = manager.LoadScript(manifest);
  ASSERT_TRUE(second.has_error());
  EXPECT_EQ(second.error().GetResult(), base::ResultCode::kALREADY_EXISTS);
  EXPECT_EQ(manager.GetAllScripts().size(), 1u) << "the refused load left something behind";
}

TEST(as_script_manager, a_manifest_without_a_name_is_refused) {
  const ScriptDir dir;
  ScriptManager manager;

  const auto manifest = dir.WriteManifest("nameless", "main_file = \"main.as\"\n", kEmptyInit);
  const auto loaded = manager.LoadScript(manifest);

  ASSERT_TRUE(loaded.has_error());
  EXPECT_EQ(loaded.error().GetResult(), base::ResultCode::kINVALID_ARGUMENT);
}

TEST(as_script_manager, a_script_whose_main_file_is_missing_is_refused) {
  const ScriptDir dir;
  ScriptManager manager;

  const auto manifest = dir.WriteManifest("gone", "name = \"gone\"\nmain_file = \"nowhere.as\"\n", kEmptyInit);
  const auto loaded = manager.LoadScript(manifest);

  ASSERT_TRUE(loaded.has_error());
  EXPECT_EQ(loaded.error().GetResult(), base::ResultCode::kNOT_FOUND);
}

TEST(as_script_manager, a_script_that_does_not_compile_is_refused_and_leaves_nothing_behind) {
  const ScriptDir dir;
  ScriptManager manager;

  const auto loaded = manager.LoadScript(dir.WriteScript("broken", kUnbuildable));

  ASSERT_TRUE(loaded.has_error());
  EXPECT_EQ(loaded.error().GetResult(), base::ResultCode::kINTERNAL_ERROR);
  EXPECT_TRUE(manager.GetAllScripts().empty());
  EXPECT_EQ(manager.GetScriptState("broken"), ScriptState::kNotLoaded);
}

TEST(as_script_manager, unloading_a_script_that_is_not_loaded_reports_it) {
  ScriptManager manager;

  const auto status = manager.UnloadScript("nothing");
  ASSERT_TRUE(status.has_error());
  EXPECT_EQ(status.error().GetResult(), base::ResultCode::kNOT_FOUND);
}

// ---------------------------------------------------------------- what a tick pass does

TEST(as_script_manager, a_tick_runs_game_init_and_then_the_game_tick_coroutine) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("steps", kYieldThenUnload)).has_error());

  // Nothing has been ticked yet, so GameInit has not run.
  EXPECT_EQ(manager.GetScriptState("steps"), ScriptState::kLoaded);

  // The first pass runs GameInit to its end - its thread::yield() has no coroutine to park - and takes
  // GameTick to its own yield, which leaves the script running and not suspended.
  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("steps"), ScriptState::kRunning);

  // The second pass resumes GameTick past the yield, which is where it unloads itself.
  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("steps"), ScriptState::kNotLoaded);
  EXPECT_TRUE(manager.GetAllScripts().empty());

  // And a pass with nothing loaded is not a crash.
  manager.TickScripts();
}

TEST(as_script_manager, a_script_parked_on_a_suspend_reads_as_suspended) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("waiting", kSuspendingTick)).has_error());

  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("waiting"), ScriptState::kSuspended);

  // Nothing wakes it, so it stays parked for as many passes as go by - and the passes cost nothing.
  const auto start = std::chrono::steady_clock::now();
  manager.TickScripts();
  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("waiting"), ScriptState::kSuspended);
  EXPECT_LT(std::chrono::steady_clock::now() - start, std::chrono::milliseconds(500)) << "ticking a suspended script waited on it";
}

TEST(as_script_manager, unloading_a_suspended_script_releases_the_coroutine_parked_in_it) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("waiting", kSuspendingTick)).has_error());
  manager.TickScripts();
  ASSERT_EQ(manager.GetScriptState("waiting"), ScriptState::kSuspended);

  // The engine is shut down by the pass, not by this call, which is what makes it safe to unload a
  // script that is parked inside its own engine from a thread that may not run it.
  ASSERT_FALSE(manager.UnloadScript("waiting").has_error());
  EXPECT_EQ(manager.GetScriptState("waiting"), ScriptState::kNotLoaded);

  manager.TickScripts();
  EXPECT_TRUE(manager.GetAllScripts().empty());
}

TEST(as_script_manager, a_script_can_unload_itself_from_game_init) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("quitter", kUnloadFromInit)).has_error());

  manager.TickScripts();

  EXPECT_EQ(manager.GetScriptState("quitter"), ScriptState::kNotLoaded);
  EXPECT_TRUE(manager.GetAllScripts().empty());

  // The next pass has nothing to run, which is the point: no GameTick started off the back of the
  // GameInit that unloaded it.
  manager.TickScripts();
  EXPECT_TRUE(manager.GetAllScripts().empty());
}

TEST(as_script_manager, a_script_can_unload_itself_from_game_tick) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("quitter", kUnloadFromTick)).has_error());
  ASSERT_EQ(manager.GetScriptState("quitter"), ScriptState::kLoaded);

  manager.TickScripts();

  // GameTick ran in the first pass it could have - the same one - and did not survive it.
  EXPECT_EQ(manager.GetScriptState("quitter"), ScriptState::kNotLoaded);
  EXPECT_TRUE(manager.GetAllScripts().empty());
}

// ----------------------------------------------------------- thread::queue_game_task

TEST(as_script_manager, a_task_queued_by_a_script_runs_while_its_script_is_alive) {
  const ScriptDir dir;
  ScriptManager manager;
  base::menu::script::GameTaskExecutor executor;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("tasker", kTaskThatUnloads)).has_error());

  // GameInit queues the task and returns; the pass does not run it, because a task is the game task
  // executor's to run and not the script's.
  manager.TickScripts();
  ASSERT_EQ(manager.GetScriptState("tasker"), ScriptState::kRunning) << "the script was not left running after GameInit";

  // One turn of the executor is enough for the task to reach its script::unload(), which ends the
  // task as well as the script - it stops rather than ticking a script that is on its way out.
  executor.Tick();

  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("tasker"), ScriptState::kNotLoaded);
  EXPECT_TRUE(manager.GetAllScripts().empty());

  // A further turn finds no task left to run, and nothing of the script's engine to run it against.
  executor.Tick();
}

TEST(as_script_manager, a_task_queued_before_its_script_is_unloaded_does_nothing) {
  const ScriptDir dir;
  ScriptManager manager;
  base::menu::script::GameTaskExecutor executor;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("tasker", kTaskThatUnloads)).has_error());
  manager.TickScripts();
  ASSERT_EQ(manager.GetScriptState("tasker"), ScriptState::kRunning);

  // The script goes before the executor ever turns the task over - which is the other half of a task
  // not outliving its script: it is the task that has to let go, since by then the engine it was
  // going to run against is already shut down.
  ASSERT_FALSE(manager.UnloadScript("tasker").has_error());
  manager.TickScripts();
  ASSERT_EQ(manager.GetScriptState("tasker"), ScriptState::kNotLoaded);

  executor.Tick();
  executor.Tick();
  EXPECT_EQ(manager.GetScriptState("tasker"), ScriptState::kNotLoaded);
}

TEST(as_script_manager, a_task_is_turned_over_once_per_executor_tick) {
  const ScriptDir dir;
  ScriptManager manager;
  base::menu::script::GameTaskExecutor executor;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("tasker", kTaskThatYields)).has_error());
  manager.TickScripts();

  // The task was queued by GameInit, and every turn of the executor carries it to its next yield -
  // nothing else would, since the script's own GameTick is not what runs it.
  for (int turn = 1; turn <= 2; ++turn) {
    executor.Tick();
    manager.TickScripts();
    EXPECT_EQ(manager.GetScriptState("tasker"), ScriptState::kRunning) << "the task ended after turn " << turn;
  }

  // The third turn is the one past the second yield, where the task unloads its own script.
  executor.Tick();
  manager.TickScripts();
  EXPECT_EQ(manager.GetScriptState("tasker"), ScriptState::kNotLoaded);
  EXPECT_TRUE(manager.GetAllScripts().empty());
}

// ----------------------------------------------------------- thread::queue_general_task

namespace {
  /// A script whose general task hands work on to a game task, and nothing else.
  ///
  /// The handoff is what makes the general thread observable here: nothing in this test ticks the
  /// script's own GameTick, so the only thing that can run either function is the general thread the
  /// manager owns - and the game task the general task leaves behind is the evidence that it did, since
  /// the game executor below runs one turn at a time and does nothing at all until one is queued.
  constexpr const char* kGeneralTaskHandoff = R"AS(
void GeneralTask() {
  thread::queue_game_task(@FinishOnTheGameThread);
}

void FinishOnTheGameThread() {
  script::unload();
}

void GameInit() {
  thread::queue_general_task(@GeneralTask);
}
)AS";
}

TEST(as_script_manager, a_general_task_runs_off_the_game_thread_and_can_hand_work_back) {
  const ScriptDir dir;
  ScriptManager manager;

  // The general thread lives in the menu's script manager, which is not the AngelScript one, and it
  // starts its loop in its own constructor.
  const base::menu::script::ScriptManager host_manager;
  auto* const general_executor = base::menu::script::kGENERAL_TASK_EXECUTOR;
  ASSERT_NE(general_executor, nullptr) << "no general task executor was registered";

  // Declared after the host manager on purpose: both register a game task executor, and the one that
  // is ticked by hand below has to be the one thread::queue_game_task reaches.
  base::menu::script::GameTaskExecutor game_executor;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("general", kGeneralTaskHandoff)).has_error());

  // GameInit is the only thing here that runs on a host pass, and it is what queues the general task.
  manager.TickScripts();
  EXPECT_TRUE(general_executor->HasPendingTasks()) << "GameInit did not queue a general task";

  // The general task goes on the general thread's queue, and the game task it queues arrives on the
  // game executor's only once it has run - so the wait is for the general queue to drain, which is the
  // general thread having run a function of this script on its own thread.
  for (int pass = 0; pass < 400 && general_executor->HasPendingTasks(); ++pass) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_FALSE(general_executor->HasPendingTasks()) << "the general thread never ran the queued task";

  ASSERT_TRUE(game_executor.HasPendingTasks())
      << "the general task did not hand its work back to the game thread";

  // From here the script ends the way any script with a queued game task does, and it can only end at
  // all because the general task ran and queued that task.
  game_executor.Tick();
  manager.TickScripts();

  EXPECT_EQ(manager.GetScriptState("general"), ScriptState::kNotLoaded)
      << "the general task never handed its work back to the game thread";
  EXPECT_TRUE(manager.GetAllScripts().empty());
}

// ---------------------------------------------------------------- waking a coroutine that is gone

// A handle is a pointer into the coroutine it came from, so one that outlives its coroutine is a
// pointer into freed memory - and unloading a script is what makes a coroutine end while another
// script may still be holding the handle it handed out. The manager is what makes that reachable,
// so the guard is pinned here: the last context's coroutine is destroyed, and the wake that follows
// has to be refused rather than followed.
namespace {
  int g_progress = 0;

  void CollectErrors(const AngelScript::asSMessageInfo* msg, void*) {
    if (msg->type == AngelScript::asMSGTYPE_ERROR) {
      ADD_FAILURE() << "the test script did not compile: " << msg->message << " (" << msg->row << ")";
    }
  }

  /// A real engine with the coro bindings, set up the way a script's own engine is.
  AngelScript::asIScriptEngine* MakeEngine() {
    auto* engine = AngelScript::asCreateScriptEngine();
    EXPECT_NE(engine, nullptr);

    g_progress = 0;
    engine->SetMessageCallback(AngelScript::asFUNCTION(CollectErrors), nullptr, AngelScript::asCALL_CDECL);

    base::menu::as::util::ClearDocs();
    base::menu::as::util::RegisterAddOns(engine);
    base::menu::as::bindings::coro::RegisterCoro(engine);
    base::menu::as::util::RegisterGlobalProperty(engine, "int g_progress", &g_progress);
    return engine;
  }

  /// A waiter that hands its handle out and parks, and a waker that wakes whatever that handle named.
  constexpr const char* kHandshake = R"AS(
thread::Handle@ g_handle;

void Waiter() {
  @g_handle = thread::handle();
  g_progress = 1;
  thread::suspend();
  g_progress = 2;
}

void Waker() {
  g_progress = 3;
  thread::wake(g_handle);
  g_progress = 4;
}
)AS";
}

TEST(as_script_manager, waking_the_handle_of_a_coroutine_that_is_gone_is_refused) {
  auto* engine = MakeEngine();

  AngelScript::asIScriptModule* mod = engine->GetModule("TestModule", AngelScript::asGM_ALWAYS_CREATE);
  mod->AddScriptSection("test.as", kHandshake);
  ASSERT_GE(mod->Build(), 0);

  auto* waiter = mod->GetFunctionByDecl("void Waiter()");
  auto* waker = mod->GetFunctionByDecl("void Waker()");
  ASSERT_NE(waiter, nullptr);
  ASSERT_NE(waker, nullptr);

  {
    ScriptContext waiting(engine);
    waiting.Run(waiter, true);
    ASSERT_TRUE(waiting.Tick());
    ASSERT_EQ(g_progress, 1);
  }
  // The context went out of scope with the coroutine parked in it, which is exactly what an unloaded
  // script does - and g_handle is now a pointer to nothing.

  ScriptContext waking(engine);
  waking.Run(waker, true);
  EXPECT_FALSE(waking.Tick());
  EXPECT_EQ(g_progress, 4) << "the wake on a handle that outlived its coroutine did not return";

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- loading and unloading off-thread

// The menu loads and unloads from its thread pool while the game thread ticks, and minicoropp lets a
// coroutine be resumed from one thread only - so the point of this is that the ticking thread is the
// only one that ever touches a running script, however the map is being changed underneath it.
TEST(as_script_manager, scripts_can_be_loaded_and_unloaded_from_other_threads_while_the_game_thread_ticks) {
  const ScriptDir dir;
  ScriptManager manager;

  constexpr int kThreads = 3;
  constexpr int kPerThread = 3;

  std::vector<ScriptManifest> manifests;
  std::vector<std::string> names;
  for (int t = 0; t < kThreads; ++t) {
    for (int i = 0; i < kPerThread; ++i) {
      const std::string name = "worker_" + std::to_string(t) + "_" + std::to_string(i);
      names.push_back(name);
      manifests.push_back(dir.WriteScript(name, kYieldingTick));
    }
  }

  std::atomic<int> finished{0};
  std::mutex failures_mutex;
  std::vector<std::string> failures;

  const auto record = [&failures, &failures_mutex](std::string message) {
    const std::scoped_lock lock(failures_mutex);
    failures.push_back(std::move(message));
  };

  std::vector<std::thread> workers;
  workers.reserve(kThreads);
  for (int t = 0; t < kThreads; ++t) {
    workers.emplace_back([&, t] {
      for (int i = 0; i < kPerThread; ++i) {
        const std::string& name = names[static_cast<std::size_t>(t * kPerThread + i)];
        const ScriptManifest& manifest = manifests[static_cast<std::size_t>(t * kPerThread + i)];

        if (const auto loaded = manager.LoadScript(manifest); loaded.has_error()) {
          record("failed to load " + name + ": " + loaded.error().GetResultMessage());
          continue;
        }

        // Read while loaded, the way the menu reads it, without ever taking a reference to the script.
        static_cast<void>(manager.GetScriptState(name));
        static_cast<void>(manager.GetAllScripts());

        if (const auto status = manager.UnloadScript(name); status.has_error()) {
          record("failed to unload " + name + ": " + status.error().GetResultMessage());
        }
      }

      finished.fetch_add(1);
    });
  }

  // The game thread, ticking whatever is loaded at the moment - including scripts a worker is
  // unloading in the middle of the pass. It has to keep ticking for as long as the workers take,
  // since a script a worker unloads is only released by a pass.
  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(30);
  while (finished.load() < kThreads && std::chrono::steady_clock::now() < deadline) {
    manager.TickScripts();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  for (auto& worker : workers) {
    worker.join();
  }

  EXPECT_TRUE(failures.empty());
  for (const auto& failure : failures) {
    ADD_FAILURE() << failure;
  }

  manager.TickScripts();
  EXPECT_TRUE(manager.GetAllScripts().empty()) << "a script outlived the worker that unloaded it";
}

// ----------------------------------------------- the reference thread::queue_game_task takes

namespace {
  /// A script that hands a function of its own to a task, and nothing else.
  constexpr const char* kQueuedTask = R"AS(
void Task() {
}

void Main() {
  thread::queue_game_task(@Task);
}
)AS";

  /// How many references to `fn` are held outside its module. AngelScript keeps that count itself, so
  /// this reads it with a balanced AddRef/Release pair - in that order, so that a count of zero is
  /// never decremented.
  int ExternalReferences(AngelScript::asIScriptFunction* fn) {
    const int c = fn->AddRef();
    fn->Release();
    return c - 1;
  }
}

// The reference a script hands over in thread::queue_game_task(@fn) belongs to the engine, and is released
// when the call returns - the binding declares the parameter as an auto handle (@+) for that reason,
// since what it keeps is the function's declaration rather than a reference to it.
//
// Getting this wrong is not a crash but a leak with a strange face: the module cannot be deleted for
// as long as the process lives, and every unload of every script that ever queued a task writes an
// "external reference to an object in module" warning from the middle of engine shutdown, where no
// test can see it as anything but log noise. The count is read here instead, at the one moment it is
// still observable.
TEST(as_script_manager, a_reference_handed_to_a_task_does_not_outlive_the_call) {
  auto* engine = MakeEngine();
  base::menu::as::bindings::game_task::RegisterGameTask(engine);

  AngelScript::asIScriptModule* mod = engine->GetModule("CreateModule", AngelScript::asGM_ALWAYS_CREATE);
  mod->AddScriptSection("test.as", kQueuedTask);
  ASSERT_GE(mod->Build(), 0);

  auto* main_fn = mod->GetFunctionByDecl("void Main()");
  auto* task = mod->GetFunctionByDecl("void Task()");
  ASSERT_NE(main_fn, nullptr);
  ASSERT_NE(task, nullptr);

  ScriptContext context(engine);
  context.Run(main_fn);

  EXPECT_EQ(ExternalReferences(task), 0)
    << "the reference thread::queue_game_task was handed is still outstanding";

  engine->ShutDownAndRelease();
}

// ---------------------------------------------------------------- a logger per script

// Every script logs through a logger of its own, named after it, writing to logs/scripts/<name>/<name>.log
// as well as the console - and the location on each line is the .as file and line the call was written
// on, not the C++ binding it went through. The logger only exists for as long as the script is loaded,
// so there is nothing here to build by hand: the manager loads one, and what it wrote is read back off
// the disk.
namespace {
  constexpr const char* kLoggingInit = R"AS(
void GameInit() {
  log::info("hello from the script");
}
)AS";

  /// Logs and unloads in the same call, so the module the line was written in is discarded in the same
  /// pass the line was logged in - before the pool has formatted it.
  constexpr const char* kLogThenUnload = R"AS(
void GameTick() {
  log::info("the last line");
  script::unload();
}
)AS";

  constexpr const char* kFirstLoggingInit = R"AS(
void GameInit() {
  log::info("from the first script");
}
)AS";

  constexpr const char* kSecondLoggingInit = R"AS(
void GameInit() {
  log::info("from the second script");
}
)AS";

  /// The 1-based line `needle` sits on, counted off the same text the script is built from - which is
  /// the line AngelScript reports for it, since a section's lines are the file's lines.
  int LineOf(const std::string& source, const std::string& needle) {
    const auto at = source.find(needle);
    if (at == std::string::npos) {
      ADD_FAILURE() << "the test source does not contain: " << needle;
      return 0;
    }

    return 1 + static_cast<int>(std::count(source.begin(), source.begin() + static_cast<std::ptrdiff_t>(at), '\n'));
  }

  /// Where a script of this name writes its own log.
  std::filesystem::path LogFileOf(const std::string& name) {
    return base::common::fs::vfs::GetScriptLogDir(name) / (name + ".log");
  }

  std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
      return {};
    }

    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
  }

  /**
   * What `name`'s log file holds, read back once it says `wanted` - or as it stands when it never does,
   * so that a failure prints what is on the disk rather than nothing.
   *
   * The write is not this thread's: the script's logger hands the line to spdlog's pool and carries on,
   * so what is waited on is the pool getting to it. The flush is this test's own - a test process has no
   * logging manager, so nothing here empties a logger's sinks on a timer the way the menu's does.
   */
  std::string ReadLogWhenItHas(const std::string& name, const std::string& wanted) {
    const auto path = LogFileOf(name);
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);

    std::string contents;
    while (std::chrono::steady_clock::now() < deadline) {
      // Gone by the time this is read in the unload case, which the file does not depend on: a logger
      // writes the lines it was already given whatever the registry says about it.
      if (const auto logger = spdlog::get(name)) {
        logger->flush();
      }

      contents = ReadFile(path);
      if (contents.find(wanted) != std::string::npos) {
        return contents;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return contents;
  }
}

TEST(as_script_manager, a_loaded_script_has_a_logger_of_its_own_and_gives_it_up_on_unload) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("logged", kEmptyInit)).has_error());
  ASSERT_NE(spdlog::get("logged"), nullptr) << "the loaded script registered no logger";

  ASSERT_FALSE(manager.UnloadScript("logged").has_error());

  // The pass is what destroys the script, and the script is what owns the logger - so this is the point
  // the name comes free again, which a further script of the same name depends on.
  manager.TickScripts();
  EXPECT_EQ(spdlog::get("logged"), nullptr) << "the unloaded script's logger is still registered";
}

TEST(as_script_manager, a_script_logs_into_its_own_file_with_the_script_location) {
  const ScriptDir dir;
  ScriptManager manager;

  // Named with a space on purpose: a script's name is its folder name, and the names in the shipped
  // examples are written the way a person writes them.
  const std::string name = "logging script";
  ASSERT_FALSE(manager.LoadScript(dir.WriteScript(name, kLoggingInit)).has_error());
  manager.TickScripts();

  const auto contents = ReadLogWhenItHas(name, "hello from the script");
  EXPECT_NE(contents.find("hello from the script"), std::string::npos) << "nothing was logged:\n" << contents;

  // One letter, because that is what the menu's own lines carry: '%L' is spdlog's abbreviated level.
  EXPECT_NE(contents.find("[I]"), std::string::npos) << contents;

  // [script name:file:line], which is where in the script the call was written.
  const auto location = name + ":main.as:" + std::to_string(LineOf(kLoggingInit, "log::info"));
  EXPECT_NE(contents.find(location), std::string::npos) << "expected " << location << " in:\n" << contents;

  // And not the line of the C++ binding it went through, which is what the location was before.
  EXPECT_EQ(contents.find("as_log.cpp"), std::string::npos) << "the line names the C++ call site:\n" << contents;
}

TEST(as_script_manager, a_script_that_logs_as_it_unloads_still_gets_its_line_written) {
  const ScriptDir dir;
  ScriptManager manager;

  const std::string name = "quitter";
  ASSERT_FALSE(manager.LoadScript(dir.WriteScript(name, kLogThenUnload)).has_error());

  // One pass runs GameTick, which logs and then unloads the script, and destroys it before the pass is
  // out. The module the line was written in is gone from here on, and so is the logger's registration.
  manager.TickScripts();
  ASSERT_EQ(manager.GetScriptState(name), ScriptState::kNotLoaded);
  EXPECT_EQ(spdlog::get(name), nullptr);

  const auto contents = ReadLogWhenItHas(name, "the last line");
  EXPECT_NE(contents.find("the last line"), std::string::npos)
      << "the line a script logged as it unloaded was dropped:\n" << contents;

  const auto location = name + ":main.as:" + std::to_string(LineOf(kLogThenUnload, "log::info"));
  EXPECT_NE(contents.find(location), std::string::npos)
      << "the location of a line logged on the way out was lost: expected " << location << " in:\n" << contents;
}

TEST(as_script_manager, two_scripts_each_log_into_their_own_file) {
  const ScriptDir dir;
  ScriptManager manager;

  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("first", kFirstLoggingInit)).has_error());
  ASSERT_FALSE(manager.LoadScript(dir.WriteScript("second", kSecondLoggingInit)).has_error());
  manager.TickScripts();

  const auto first = ReadLogWhenItHas("first", "from the first script");
  const auto second = ReadLogWhenItHas("second", "from the second script");

  EXPECT_NE(first.find("from the first script"), std::string::npos) << first;
  EXPECT_NE(second.find("from the second script"), std::string::npos) << second;

  // Neither file holds a line of the other's, which is the whole of what a logger per script is for.
  EXPECT_EQ(first.find("from the second script"), std::string::npos) << first;
  EXPECT_EQ(second.find("from the first script"), std::string::npos) << second;

  // Each line still says which script and which file it came from, in its own file as much as in the
  // other's - the location is the logger's, not a heading on the file.
  EXPECT_NE(first.find("first:main.as:" + std::to_string(LineOf(kFirstLoggingInit, "log::info"))), std::string::npos) << first;
  EXPECT_NE(second.find("second:main.as:" + std::to_string(LineOf(kSecondLoggingInit, "log::info"))), std::string::npos) << second;
}
