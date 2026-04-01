//
// Created by X-ray on 01/04/2026.
//

#include <gtest/gtest.h>
#include <base-coro/coroutine.hpp>
#include <base-coro/mutex.hpp>
#include <chrono>

using namespace minicoropp;

TEST(CoroutineTest, ExecutionAndState) {
  bool executed = false;
  const Coroutine coro([&] {
    executed = true;
  });

  EXPECT_EQ(coro.state(), CoroState::kSUSPENDED);
  const auto res = coro.resume();
  EXPECT_EQ(res, CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kDEAD);
  EXPECT_TRUE(executed);
}

TEST(CoroutineTest, UserDataPassing) {
  int data = 42;
  const Coroutine coro([&] {
    auto* usr_data = static_cast<int*>(this_coro::get_data());
    EXPECT_NE(usr_data, nullptr);
    if (usr_data) {
      EXPECT_EQ(*usr_data, 42);
    }
  }, &data);

  coro.resume();
}

TEST(CoroutineTest, Yielding) {
  int counter = 0;
  const Coroutine coro([&] {
    counter++;
    this_coro::yield();
    counter++;
  });

  EXPECT_EQ(counter, 0);
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(counter, 1);
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(counter, 2);
  EXPECT_EQ(coro.state(), CoroState::kDEAD);
}

TEST(CoroutineTest, Sleeping) {
  const Coroutine coro([] {
    this_coro::sleep_for(std::chrono::milliseconds(50));
  });

  // First resume runs up to the sleep_for (which calls yield internally)
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kSUSPENDED);

  // The sleep timer should block immediate resume
  EXPECT_EQ(coro.resume(), CoroResult::kYIELDING);

  // Wait beyond the sleep duration
  std::this_thread::sleep_for(std::chrono::milliseconds(60));

  // Now it should successfully resume and finish
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kDEAD);
}

TEST(CoroutineTest, Exceptions) {
  const Coroutine coro([] {
    throw std::runtime_error("test exception");
  });

  // Resuming throws the exception inside the wrapper, which captures it
  EXPECT_EQ(coro.resume(), CoroResult::kEXCEPTION);
  EXPECT_NE(coro.exception(), nullptr);

  try {
    std::rethrow_exception(coro.exception());
  } catch (const std::exception& e) {
    EXPECT_STREQ(e.what(), "test exception");
  }
}

TEST(CoroutineMutexTest, TryLockUncontested) {
  Mutex mtx;
  EXPECT_TRUE(mtx.try_lock());
  mtx.unlock();
}

TEST(CoroutineMutexTest, SuspensionsAndWakes) {
  Mutex mtx;
  int shared = 0;

  const Coroutine c1([&] {
    mtx.lock();
    shared = 1;
    this_coro::yield(); // hold the lock safely through a generic yield
    shared = 2;
    mtx.unlock(); // hand lock to queued waiters
  });

  const Coroutine c2([&] {
    mtx.lock(); // Suspend indefinitely since c1 holds it
    shared = 3;
    mtx.unlock();
  });

  // Start c1. It acquires the lock, sets to 1, and yields normally.
  EXPECT_EQ(c1.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(shared, 1);

  // Start c2. It attempts to lock, fails, queues itself and suspends.
  EXPECT_EQ(c2.resume(), CoroResult::kSUCCESS); // First resume hits suspend -> yield successful

  // Further premature resumptions to c2 correctly yield instantly without executing
  EXPECT_EQ(c2.resume(), CoroResult::kYIELDING);

  // Resume c1. Sets to 2 and unlocks, effectively restoring c2's wake condition!
  EXPECT_EQ(c1.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(shared, 2);
  EXPECT_EQ(c1.state(), CoroState::kDEAD);

  // Resume c2. It can now successfully wake up and finish execution!
  EXPECT_EQ(c2.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(shared, 3);
  EXPECT_EQ(c2.state(), CoroState::kDEAD);
}

TEST(CoroutineTest, MoveSemantics) {
  int value = 0;
  Coroutine coro1([&] { value = 1; });

  // Move constructor
  Coroutine coro2(std::move(coro1));

  // coro1 should be empty now
  EXPECT_EQ(coro1.state(), CoroState::kDEAD);
  EXPECT_EQ(coro1.resume(), CoroResult::kINVALID_COROUTINE);

  // coro2 should have the state
  EXPECT_EQ(coro2.state(), CoroState::kSUSPENDED);
  EXPECT_EQ(coro2.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(value, 1);
  EXPECT_EQ(coro2.state(), CoroState::kDEAD);

  // Move assignment
  Coroutine coro3([&] { value = 2; });
  coro3 = std::move(coro2); // coro2 is dead, moving it into coro3

  EXPECT_EQ(coro3.state(), CoroState::kDEAD);
  EXPECT_EQ(coro2.state(), CoroState::kDEAD);
}

TEST(CoroutineTest, SleepUntil) {
  const Coroutine coro([] {
    const auto wake_time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50);
    this_coro::sleep_until(wake_time);
  });

  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kSUSPENDED);

  // Still waiting
  EXPECT_EQ(coro.resume(), CoroResult::kYIELDING);

  std::this_thread::sleep_for(std::chrono::milliseconds(60));

  // Timer naturally expired
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kDEAD);
}

TEST(CoroutineTest, ResumeAfterDead) {
  const Coroutine coro([]{});

  // Normal execution
  EXPECT_EQ(coro.resume(), CoroResult::kSUCCESS);
  EXPECT_EQ(coro.state(), CoroState::kDEAD);

  // Resuming a finished coroutine returns an error indicating it's not suspended
  EXPECT_EQ(coro.resume(), CoroResult::kNOT_SUSPENDED);
}

TEST(CoroutineTest, NonCoroutineContext) {
  // Calling these outside of a resumed coroutine context should safely fall back
  // to the main thread's equivalent functions or safely return nullptrs, without crashing.
  EXPECT_NO_THROW({
    this_coro::yield();
    this_coro::sleep_for(std::chrono::milliseconds(1));
    this_coro::sleep_until(std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1));
  });

  EXPECT_EQ(this_coro::get_data(), nullptr);
  EXPECT_EQ(this_coro::get_current_handle(), nullptr);
}
