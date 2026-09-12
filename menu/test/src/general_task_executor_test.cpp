//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>
#include <base-common/win32/signal.hpp>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <future>
#include <thread>
#include <vector>

#include <minicoropp/coroutine.hpp>

#include "../../src/script/general_task_executor.hpp"
#include "../../src/script/script_manager.hpp"

// The general thread is the one loop in the menu that owns its thread rather than being ticked by the
// menu or the game, and both halves of that have to hold: what is queued onto it runs on that thread
// and nowhere else, and a thread with nothing to do must cost nothing rather than spin. The second
// half is the one this file is really about - a loop that spins passes every test about running tasks
// and is still wrong - so where a test can tell the two apart it does, by the clock rather than by the
// task.
namespace {
  using base::menu::script::GeneralTaskExecutor;
  using base::menu::script::ScriptManager;
  using base::win32::Signal;

  /// How long a test waits for the general thread before it says it never came back. The loop's own
  /// cadence is `kGENERAL_TICK_MS`, so this is two orders of magnitude past anything but a hung thread.
  constexpr auto kGenerousWait = std::chrono::seconds(5);

  // ------------------------------------------------------------------ the thread the manager owns

  TEST(general_task_executor, a_queued_task_runs_on_the_general_thread) {
    ScriptManager manager;

    auto* const executor = base::menu::script::kGENERAL_TASK_EXECUTOR;
    ASSERT_NE(executor, nullptr) << "the manager did not register a general task executor";

    const auto caller_thread = std::this_thread::get_id();
    std::atomic<std::thread::id> task_thread{};
    std::atomic<bool> ran{false};

    auto future = executor->QueueTask([&] {
      task_thread = std::this_thread::get_id();
      ran = true;
    });

    ASSERT_EQ(future.wait_for(kGenerousWait), std::future_status::ready) << "the general thread never ran the queued task";

    EXPECT_TRUE(ran.load());
    EXPECT_NE(task_thread.load(), caller_thread) << "the task ran on the thread that queued it";
  }

  TEST(general_task_executor, a_task_queued_while_the_thread_is_idle_is_picked_up_at_once) {
    ScriptManager manager;
    auto* const executor = base::menu::script::kGENERAL_TASK_EXECUTOR;
    ASSERT_NE(executor, nullptr);

    // Long enough that the loop has gone round its first passes and parked on the signal rather than
    // still starting up: what is being timed below is the wake, not the thread's first tick.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    const auto queued_at = std::chrono::steady_clock::now();
    auto future = executor->QueueTask([] {});

    EXPECT_EQ(future.wait_for(kGenerousWait), std::future_status::ready);
    const auto waited = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - queued_at);

    // An idle loop that polled at its own cadence would still be within this bound, so this is not the
    // proof - the proof is below, where the wait has nothing but the notify to end it.
    EXPECT_LT(waited.count(), 1000) << "an idle general thread took " << waited.count() << "ms to pick up a task";
  }

  TEST(general_task_executor, queueing_a_task_wakes_a_thread_parked_on_the_signal) {
    // What an idle loop parks on is the signal, and the executor's half of that contract is that it
    // rings it for every queued task. Parked here means a wait with no notify that could end it: the
    // cap is what tells the two apart, since a pass that came back because the cap expired takes the
    // whole cap while a pass that was woken does not.
    constexpr std::int32_t kParkedWaitMs = 2000;
    constexpr std::int32_t kExpectedWakeMs = 500;

    Signal wake;
    GeneralTaskExecutor executor(&wake);

    std::atomic<bool> stop{false};
    std::atomic<bool> ran{false};
    std::thread loop([&] {
      while (!stop) {
        executor.Tick();
        wake.Wait(kParkedWaitMs);
      }
    });

    const auto queued_at = std::chrono::steady_clock::now();
    auto future = executor.QueueTask([&ran] {
      ran = true;
    });

    ASSERT_EQ(future.wait_for(kGenerousWait), std::future_status::ready) << "the parked loop never ran the task";
    const auto waited = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - queued_at);

    EXPECT_TRUE(ran.load());
    EXPECT_LT(waited.count(), kExpectedWakeMs)
        << "the task waited " << waited.count() << "ms, which is the park timing out rather than the notify waking it";

    // Teardown: this notify is the test's, to end the park the loop went back into after that pass.
    stop = true;
    wake.Notify();
    loop.join();
  }

  TEST(general_task_executor, destroying_the_manager_with_a_task_still_queued_does_not_wait_for_it) {
    // The thread has to be gone before the hosts it ticks are, and it is parked while it has nothing
    // to do - so the teardown is a notify, not a wait for the next cadence. A task that never ends is
    // what makes that matter: it is the one thing that would keep the loop from parking.
    GeneralTaskExecutor* executor = nullptr;
    {
      ScriptManager manager;
      executor = base::menu::script::kGENERAL_TASK_EXECUTOR;
      ASSERT_NE(executor, nullptr);

      executor->QueueTask([] {
        while (true) {
          minicoropp::this_coro::yield();
        }
      });

      // Long enough for the loop to have taken the task up and gone round its cadence at least once,
      // which is the state the destructor below has to get out of.
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_EQ(base::menu::script::kGENERAL_TASK_EXECUTOR, nullptr) << "the executor outlived the manager that owned it";
  }

  // ------------------------------------------------------------------ the queue itself

  // The rest of the file drives one executor by hand rather than through the manager's loop, which is
  // what makes the passes and what each of them runs the only thing in play. A null signal is an
  // executor with no loop to wake, which is exactly what a hand-ticked one wants.
  TEST(general_task_executor, a_yielding_task_is_resumed_on_a_later_pass) {
    GeneralTaskExecutor executor(nullptr);

    std::atomic<int> progress{0};
    auto future = executor.QueueTask([&progress] {
      progress = 1;
      minicoropp::this_coro::yield();
      progress = 2;
      minicoropp::this_coro::yield();
      progress = 3;
    });

    EXPECT_TRUE(executor.HasPendingTasks()) << "a queued task is not reported as pending";

    executor.Tick();
    EXPECT_EQ(progress.load(), 1) << "the first pass did not run the task";
    EXPECT_EQ(future.wait_for(std::chrono::milliseconds(0)), std::future_status::timeout) << "the task finished in one pass";

    // A task that yielded is still queued, which is what keeps the loop from parking while it waits:
    // the pass after this one is what carries it past the yield.
    EXPECT_TRUE(executor.HasPendingTasks()) << "a task that yielded is no longer pending";

    executor.Tick();
    EXPECT_EQ(progress.load(), 2);

    executor.Tick();
    EXPECT_EQ(progress.load(), 3);

    EXPECT_FALSE(executor.HasPendingTasks()) << "a finished task is still queued";
    EXPECT_EQ(future.wait_for(std::chrono::milliseconds(0)), std::future_status::ready) << "a finished task left its future unsettled";
  }

  TEST(general_task_executor, a_task_can_queue_another_task_from_inside_itself) {
    GeneralTaskExecutor executor(nullptr);

    std::atomic<bool> inner_ran{false};
    std::atomic<bool> outer_ran{false};

    // A task queueing a task is the case a pass that resumed inside its own lock would deadlock on,
    // and it is not a hypothetical: it is how anything hands work on from work.
    executor.QueueTask([&executor, &inner_ran, &outer_ran] {
      outer_ran = true;
      executor.QueueTask([&inner_ran] {
        inner_ran = true;
      });
    });

    executor.Tick();
    EXPECT_TRUE(outer_ran.load()) << "the first pass did not run the outer task";
    EXPECT_FALSE(inner_ran.load()) << "the task queued during a pass was run in the same pass";

    executor.Tick();
    EXPECT_TRUE(inner_ran.load()) << "the task queued during a pass was never run";
    EXPECT_FALSE(executor.HasPendingTasks());
  }

  TEST(general_task_executor, tasks_queued_from_many_threads_each_run_exactly_once) {
    GeneralTaskExecutor executor(nullptr);

    constexpr int kThreads = 8;
    constexpr int kPerThread = 25;
    std::atomic<int> ran{0};

    std::vector<std::thread> threads;
    threads.reserve(kThreads);
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&executor, &ran] {
        for (int j = 0; j < kPerThread; ++j) {
          executor.QueueTask([&ran] {
            ran.fetch_add(1);
          });
        }
      });
    }

    for (auto& thread : threads) {
      thread.join();
    }

    ASSERT_TRUE(executor.HasPendingTasks());
    EXPECT_EQ(ran.load(), 0) << "a task ran before the executor was ticked";

    // Every one of them runs on one pass, since none of them yields, and none of them runs twice -
    // which is what the count says once the queue has drained.
    for (int pass = 0; pass < 4 && executor.HasPendingTasks(); ++pass) {
      executor.Tick();
    }

    EXPECT_EQ(ran.load(), kThreads * kPerThread) << "a task was run more or fewer times than it was queued";
    EXPECT_FALSE(executor.HasPendingTasks());
  }
}
