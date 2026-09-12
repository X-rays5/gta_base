//
// Created by X-ray on 12/09/2026.
//
#include <base-common/util/callable.hpp>
#include <gtest/gtest.h>

#include <array>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {
  // Counts how a Tracked value crossed a parameter boundary, so that argument
  // forwarding can be asserted exactly instead of inferred from a move-only type
  // merely compiling.
  struct MoveCounter {
    int copies = 0;
    int moves = 0;
  };

  struct Tracked {
    explicit Tracked(MoveCounter& counter)
        : counter_(&counter) {}

    Tracked(const Tracked& other) noexcept
        : counter_(other.counter_) { ++counter_->copies; }

    Tracked(Tracked&& other) noexcept
        : counter_(other.counter_) { ++counter_->moves; }

    Tracked& operator=(const Tracked&) = delete;
    Tracked& operator=(Tracked&&) = delete;

  private:
    MoveCounter* counter_;
  };

  int AddFree(const int a, const int b) {
    return a + b;
  }

  auto zero_arg = [] { return 42; };

  struct Accumulator {
    int total = 0;

    int Add(const int value) { return total += value; }
  };

  // Distinguishes the value category of the argument it is handed.
  struct Overloaded {
    std::string operator()(std::string&) const { return "lvalue"; }
    std::string operator()(std::string&&) const { return "rvalue"; }
  };

  int AddOneFree(const int value) {
    return value + 1;
  }

  struct Offset {
    int by = 0;

    int operator()(const int value) const { return value + by; }
  };

  struct LifetimeCounters {
    int live = 0;
    int copies = 0;
    int moves = 0;
  };

  // Reports its own construction and destruction, so that a leak or a double
  // destruction in the inline storage shows up as a wrong count instead of as a
  // silently corrupted heap.
  struct Probe {
    explicit Probe(LifetimeCounters& counters, const int value = 0)
        : counters_(&counters), value_(value) { ++counters_->live; }

    Probe(const Probe& other)
        : counters_(other.counters_), value_(other.value_) { ++counters_->live; ++counters_->copies; }

    Probe(Probe&& other) noexcept
        : counters_(other.counters_), value_(other.value_) { ++counters_->live; ++counters_->moves; }

    Probe& operator=(const Probe&) = delete;
    Probe& operator=(Probe&&) = delete;

    ~Probe() { --counters_->live; }

    int operator()(const int arg) const { return value_ + arg; }

  private:
    LifetimeCounters* counters_;
    int value_;
  };
}

using namespace base::common::util;

TEST(Callback, HoldsOnlyTheExactCallableTypeItWasBuiltFor) {
  auto add_one = [](const int value) { return value + 1; };
  using Cb = Callback<decltype(add_one)>;

  // The lambda's own closure type is what the wrapper is parameterized on, so another
  // value of that same type is accepted.
  static_assert(std::is_constructible_v<Cb, decltype(add_one)>);
  // Nothing else is, not even a second lambda written identically: every lambda
  // expression has a closure type of its own.
  static_assert(!std::is_constructible_v<Cb, int (*)(int)>);
  static_assert(!std::is_constructible_v<Cb, decltype([](const int value) { return value + 1; })>);

  const auto lambda = add_one;
  const Cb cb{lambda};

  EXPECT_EQ(cb(41), 42);
}

TEST(Callback, StoresCapturedStateInTheWrapper) {
  int by_reference = 0;
  Callback write_through{[&by_reference](const int value) { by_reference = value; }};
  Callback count_calls{[calls = 0](const int value) mutable { return value + calls++; }};

  write_through(7);
  EXPECT_EQ(by_reference, 7) << "A by-reference capture should reach the original object.";

  // A by-value capture is a member of the stored closure, so it outlives the statement
  // that produced the lambda and lives exactly as long as the Callback does.
  EXPECT_EQ(count_calls(0), 0);
  EXPECT_EQ(count_calls(0), 1) << "Captured state should persist across calls.";
}

TEST(Callback, StoresMoveOnlyCaptures) {
  auto lambda = [value = std::make_unique<int>(42)] { return *value; };

  const Callback cb{std::move(lambda)};

  EXPECT_EQ(cb(), 42) << "The closure should be moved into the wrapper, not copied.";
}

TEST(Callback, DeducesCallableTypeFromLambda) {
  const auto lambda = [](const int value) { return value * 2; };

  const Callback cb{lambda};

  // F is deduced from a by-value parameter, so the lambda's const is stripped.
  static_assert(std::is_same_v<std::remove_cvref_t<decltype(cb)>, Callback<std::remove_cvref_t<decltype(lambda)>>>);
  EXPECT_EQ(cb(21), 42);
}

TEST(Callback, DeducesFunctionPointerFromFreeFunction) {
  const Callback cb{AddFree};

  // A function lvalue decays to a function pointer when deduced by value.
  static_assert(std::is_same_v<std::remove_cvref_t<decltype(cb)>, Callback<int (*)(int, int)>>);
  EXPECT_EQ(cb(20, 22), 42);
}

TEST(Callback, InvokesCallableWithNoArguments) {
  const Callback cb{zero_arg};

  EXPECT_EQ(cb(), 42);
}

TEST(Callback, ForwardsArgumentsInOrder) {
  const Callback cb{[](const int a, const std::string& b, const double c) {
    return std::to_string(a) + b + std::to_string(static_cast<int>(c));
  }};

  EXPECT_EQ(cb(1, ":", 2.0), "1:2");
}

TEST(Callback, PassesLvalueReferenceToNonConstParameter) {
  const Callback cb{[](int& slot, const int value) { slot = value; }};
  int slot = 0;

  cb(slot, 5);

  EXPECT_EQ(slot, 5);
}

TEST(Callback, PreservesArgumentValueCategory) {
  const Callback cb{Overloaded{}};
  std::string lvalue = "x";

  EXPECT_EQ(cb(lvalue), "lvalue");
  EXPECT_EQ(cb(std::string{"x"}), "rvalue");
}

TEST(Callback, MovesRvalueArgumentsAndCopiesLvalueArguments) {
  const Callback cb{[](Tracked) {}};
  MoveCounter counter;
  const Tracked item{counter};

  cb(Tracked{counter});
  EXPECT_EQ(counter.copies, 0);
  EXPECT_EQ(counter.moves, 1) << "An rvalue argument should be moved, not copied.";

  cb(item);
  EXPECT_EQ(counter.copies, 1) << "An lvalue argument should be copied, not moved.";
  EXPECT_EQ(counter.moves, 1);
}

TEST(Callback, AcceptsMoveOnlyArguments) {
  const Callback cb{[](std::unique_ptr<int> value) { return *value; }};

  EXPECT_EQ(cb(std::make_unique<int>(7)), 7);
}

TEST(Callback, PreservesReturnType) {
  int value = 10;
  const Callback cb{[&value]() -> int& { return value; }};

  static_assert(std::is_same_v<decltype(cb()), int&>);

  cb() = 42;
  EXPECT_EQ(value, 42) << "Writing through the returned reference should be observable.";
}

TEST(Callback, SupportsVoidReturningCallables) {
  int captured = 0;
  const Callback cb{[&captured](const int value) { captured = value; }};

  static_assert(std::is_same_v<decltype(cb(1)), void>);

  cb(9);
  EXPECT_EQ(captured, 9);
}

TEST(Callback, InvokableOnConstLvalueAndRvalue) {
  Callback cb{[](const int value) { return value + 1; }};
  const auto& const_ref = cb;

  EXPECT_EQ(const_ref(1), 2) << "A const callable should be invocable on a const Callback.";
  EXPECT_EQ(cb(2), 3);
  EXPECT_EQ(std::move(cb)(3), 4) << "Deducing this should allow invoking an rvalue Callback.";
}

TEST(Callback, MutableCallableIsInvokableThroughNonConstCallback) {
  Callback cb{[count = 0](const int value) mutable { return count += value; }};

  EXPECT_EQ(cb(1), 1);
  EXPECT_EQ(cb(2), 3) << "State mutations should persist across calls.";
}

TEST(Callback, MutableCallableIsRejectedOnConstCallback) {
  auto mutable_lambda = [count = 0]() mutable { return count++; };
  using Cb = Callback<decltype(mutable_lambda)>;

  static_assert(std::invocable<Cb&>);
  static_assert(!std::invocable<const Cb&>);
}

TEST(Callback, ConstraintRejectsIncompatibleArguments) {
  const Callback zero_arg_cb{zero_arg};
  const Callback binary_cb{AddFree};

  static_assert(std::invocable<decltype(zero_arg_cb)&>);
  static_assert(!std::invocable<decltype(zero_arg_cb)&, int>, "Too many arguments should be rejected.");
  static_assert(std::invocable<decltype(binary_cb)&, int, int>);
  static_assert(!std::invocable<decltype(binary_cb)&, int>, "Too few arguments should be rejected.");
  static_assert(!std::invocable<decltype(binary_cb)&, int, std::string>, "Wrong argument types should be rejected.");

  EXPECT_EQ(zero_arg_cb(), 42);
}

TEST(Callback, InvokesMemberFunctionPointers) {
  const Callback cb{&Accumulator::Add};
  Accumulator accumulator;

  EXPECT_EQ(cb(accumulator, 5), 5);
  EXPECT_EQ(cb(&accumulator, 3), 8) << "std::invoke should accept both a reference and a pointer.";
}

TEST(Callback, WorksWithStdFunction) {
  const std::function<int(int)> fn = [](const int value) { return value * 3; };
  const Callback cb{fn};

  EXPECT_EQ(cb(14), 42);
}

TEST(Callback, PropagatesExceptions) {
  const Callback cb{[]() -> int { throw std::runtime_error("boom"); }};

  EXPECT_THROW(cb(), std::runtime_error);
}

TEST(Callback, MovedCallbackRemainsInvokable) {
  Callback cb{[](const int value) { return value + 1; }};
  const Callback moved{std::move(cb)};

  EXPECT_EQ(moved(41), 42);
}

TEST(Callback, UsableInConstantExpressions) {
  constexpr auto add = [](const int a, const int b) constexpr { return a + b; };
  constexpr Callback cb{add};
  constexpr int result = cb(40, 2);

  static_assert(result == 42);
  EXPECT_EQ(result, 42);
}

TEST(CallbackSignature, HoldsCallablesOfDifferentTypesWithOneSignature) {
  std::vector<Callback<int(int)>> callbacks;

  callbacks.emplace_back([](const int value) { return value + 1; });
  callbacks.emplace_back(AddOneFree);
  callbacks.emplace_back(Offset{10});
  callbacks.emplace_back([offset = 100](const int value) { return value + offset; });

  ASSERT_EQ(callbacks.size(), 4);

  int total = 0;
  for (auto& callback : callbacks) {
    ASSERT_TRUE(callback);
    total += callback(1);
  }

  EXPECT_EQ(total, 2 + 2 + 11 + 101);
}

TEST(CallbackSignature, KeepsTargetsIntactAcrossReallocation) {
  std::vector<Callback<int(int)>> callbacks;

  // Growth moves every stored target, so the mix of two distinct types has to
  // survive being relocated.
  for (int i = 0; i < 16; ++i) {
    if (i % 2 == 0) {
      callbacks.emplace_back(Offset{i});
    } else {
      callbacks.emplace_back([i](const int value) { return value + i; });
    }
  }

  int total = 0;
  for (auto& callback : callbacks) {
    total += callback(0);
  }

  EXPECT_EQ(total, 120) << "0 to 15 should still sum to 120 after the vector grew.";
}

TEST(CallbackSignature, SupportsVoidReturn) {
  std::vector<Callback<void()>> callbacks;
  int first = 0;
  int second = 0;

  callbacks.push_back([&first] { first = 1; });
  callbacks.push_back([&second] { second = 2; });

  for (auto& callback : callbacks) {
    callback();
  }

  EXPECT_EQ(first, 1);
  EXPECT_EQ(second, 2);
}

TEST(CallbackSignature, EmptinessIsTestedExplicitly) {
  static_assert(!std::is_convertible_v<Callback<int()>, bool>);
  static_assert(std::is_constructible_v<bool, Callback<int()>>);

  EXPECT_FALSE(Callback<int()>{});
}

TEST(CallbackSignature, MoveConstructionTransfersTheTargetExactlyOnce) {
  LifetimeCounters counters;
  Callback<int(int)> source{Probe{counters, 5}};
  ASSERT_EQ(counters.live, 1);

  {
    Callback<int(int)> moved{std::move(source)};

    EXPECT_EQ(counters.live, 1) << "The target should be moved, never copied or destroyed twice.";
    EXPECT_EQ(counters.copies, 0);
    EXPECT_FALSE(source) << "A moved-from Callback should be empty.";
    EXPECT_EQ(moved(1), 6);
  }

  EXPECT_EQ(counters.live, 0) << "The target should be destroyed exactly once.";
}

TEST(CallbackSignature, DestroysTheTargetOnDestruction) {
  LifetimeCounters counters;

  {
    const Callback<int(int)> callback{Probe{counters, 1}};
    EXPECT_EQ(counters.live, 1);
  }

  EXPECT_EQ(counters.live, 0);
}

TEST(CallbackSignature, MoveAssignmentDestroysTheReplacedTarget) {
  LifetimeCounters counters;
  Callback<int(int)> target{Probe{counters, 1}};

  {
    Callback<int(int)> source{Probe{counters, 2}};
    target = std::move(source);

    EXPECT_EQ(counters.live, 1) << "The replaced target should have been destroyed.";
    EXPECT_FALSE(source);
    EXPECT_EQ(target(0), 2);
  }

  EXPECT_EQ(counters.live, 1);
  EXPECT_EQ(target(0), 2) << "The assigned target should outlive the source's scope.";
}

TEST(CallbackSignature, SelfMoveAssignmentLeavesTheTargetIntact) {
  Callback<int(int)> callback{[](const int value) { return value * 2; }};
  auto& same = callback;

  callback = std::move(same);

  EXPECT_TRUE(callback);
  EXPECT_EQ(callback(21), 42);
}

TEST(CallbackSignature, InvokesMutableTargetThroughNonConstCallback) {
  Callback<int()> callback{[calls = 0]() mutable { return ++calls; }};

  EXPECT_EQ(callback(), 1);
  EXPECT_EQ(callback(), 2) << "Target state should persist across calls.";
}

TEST(CallbackSignature, InvokesConstTargetThroughConstCallback) {
  int calls = 0;
  const Callback<int() const> callback{[&calls] { return ++calls; }};

  EXPECT_EQ(callback(), 1);
  EXPECT_EQ(callback(), 2);
}

TEST(CallbackSignature, ConstSignatureAllowsConstIteration) {
  std::vector<Callback<int(int) const>> callbacks;

  callbacks.emplace_back([](const int value) { return value + 1; });
  callbacks.emplace_back(Offset{10});

  int total = 0;
  for (const auto& callback : callbacks) {
    total += callback(1);
  }

  EXPECT_EQ(total, 13);
}

TEST(CallbackSignature, ConstnessOfTheSignatureDecidesInvocability) {
  // The plain signature's operator() is non-const, so a const Callback is not callable.
  static_assert(std::is_invocable_v<Callback<int()>&>);
  static_assert(std::is_invocable_v<Callback<int()>>);
  static_assert(!std::is_invocable_v<const Callback<int()>&>);

  // The const signature is callable either way.
  static_assert(std::is_invocable_v<Callback<int() const>&>);
  static_assert(std::is_invocable_v<const Callback<int() const>&>);
}

TEST(CallbackSignature, ConstSignatureRejectsNonConstInvocableTargets) {
  static_assert(std::is_constructible_v<Callback<int()>, decltype([] { return 0; })>);
  static_assert(std::is_constructible_v<Callback<int() const>, decltype([] { return 0; })>);

  // A mutable lambda that carries state has no const path, so it is refused.
  static_assert(std::is_constructible_v<Callback<int()>, decltype([calls = 0]() mutable { return ++calls; })>);
  static_assert(!std::is_constructible_v<Callback<int() const>, decltype([calls = 0]() mutable { return ++calls; })>);

  // A captureless mutable lambda is still const-invocable, through the surrogate call
  // function contributed by its conversion to a function pointer: there is no state to
  // mutate, so the const signature has nothing to protect.
  static_assert(std::is_constructible_v<Callback<int() const>, decltype([]() mutable { return 0; })>);
}

TEST(CallbackSignature, InvokesCapturelessMutableLambdaThroughConstCallback) {
  // The const path here is the function pointer conversion, not a const operator(), so
  // it has to be confirmed to return what the lambda computes.
  const Callback<int(int) const> callback{[](int value) mutable { return value + 1; }};

  EXPECT_EQ(callback(41), 42);
}

TEST(CallbackSignature, AcceptsAnErasedWrapperAsATarget) {
  // The wrapper and the erased versions are unrelated types, but the wrapper is a
  // perfectly good target: it satisfies whichever erased signature its own callable can
  // be invoked through. Nothing here is special-cased for it.
  auto multiply = [](const int value) { return value * 2; };
  const Callback wrapper{multiply};

  const Callback<int(int) const> erased{wrapper};

  EXPECT_EQ(erased(21), 42);

  // Symmetrically, a wrapper around a mutable callable only satisfies the plain
  // signature: the wrapper's operator() is non-const, so there is no const path to it.
  auto increment = [count = 0](const int value) mutable { return value + count++; };
  using MutableWrapper = Callback<decltype(increment)>;

  static_assert(std::is_constructible_v<Callback<int(int)>, MutableWrapper>);
  static_assert(!std::is_constructible_v<Callback<int(int) const>, MutableWrapper>);
}

TEST(CallbackSignature, ForwardsArgumentsWithoutCopyingThem) {
  Callback<void(std::string&, const std::string&)> callback{
    [](std::string& out, const std::string& in) { out = in; }
  };
  std::string out;

  callback(out, "hello");

  EXPECT_EQ(out, "hello");
}

TEST(CallbackSignature, ConvertsArgumentsToTheSignature) {
  // The target takes long, the signature says int: the conversion happens at the
  // call boundary, exactly as it would for a direct call.
  Callback<std::string(int)> callback{[](const long value) { return std::to_string(value); }};

  EXPECT_EQ(callback(42), "42");
}

TEST(CallbackSignature, InvokesMemberFunctionPointers) {
  Callback<int(Accumulator&, int)> callback{&Accumulator::Add};
  Accumulator accumulator;

  EXPECT_EQ(callback(accumulator, 5), 5);
  EXPECT_EQ(callback(accumulator, 3), 8);
}

TEST(CallbackSignature, CapturesHaveToFitTheInlineStorage) {
  // 64 bytes of captured state is past the erased storage's 32, so the erased form
  // refuses the closure outright. The wrapper has no such limit: it holds the closure
  // itself, whatever its size.
  auto fat = [blob = std::array<char, 64>{}] { return blob.size(); };

  static_assert(sizeof(fat) > 32);
  static_assert(std::is_constructible_v<Callback<decltype(fat)>, decltype(fat)>);

  Callback wrapper{std::move(fat)};
  EXPECT_EQ(wrapper(), 64);

  // The erased form refuses it instead. Uncomment to see how that reads: the failure is
  // reported as a bare "static assertion failed" at this line, with neither the reason
  // nor a location in the header.
  // Callback<int()> erased{std::move(fat)};
}

TEST(CallbackSignature, AcceptsMoveOnlyTargets) {
  Callback<int()> callback{[value = std::make_unique<int>(42)] { return *value; }};

  EXPECT_EQ(callback(), 42);
}

TEST(CallbackSignature, DoesNotDecayRvalueReferenceParameters) {
  MoveCounter counter;
  // Args is Tracked&&, so the parameter is Tracked&&: the argument binds straight
  // through to the target without a by-value parameter in between.
  Callback<void(Tracked&&)> callback{[](Tracked&&) {}};

  callback(Tracked{counter});

  EXPECT_EQ(counter.copies, 0);
  EXPECT_EQ(counter.moves, 0) << "An rvalue reference argument should bind directly, not decay to a by-value copy.";
}

TEST(CallbackSignature, ForwardsValueParametersAsRvalues) {
  // Args is a value type, so the parameter is a local copy that is forwarded as an
  // rvalue, exactly as std::function does.
  Callback<std::string(std::string)> callback{[](std::string&& value) { return value + "!"; }};

  EXPECT_EQ(callback(std::string{"hi"}), "hi!");
}
